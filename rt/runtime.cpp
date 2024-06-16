#include <cstddef>
#include <exception>
#include <memory>
#include <string_view>

#include <fmt/core.h>
#include <lua.hpp>
#include <uv.h>

#include "lua/helpers.hpp"
#include "modules/task.hpp"
#include "runtime.hpp"

namespace lua = lege::lua;

namespace lege {

Runtime::Runtime() : L() {
  // Check that the loaded libuv is compatible with the version we were compiled
  // with
  unsigned uvLibVersion = uv_version();
  // We ignore the patch number as these should be semver compatible
  if ((uvLibVersion & ~0xff) != (UV_VERSION_HEX & ~0xff)) {
    throw std::runtime_error(
        fmt::format("Incompatible version of libuv: compiled with {}.{}.{}{}, "
                    "but loaded version is {}",
                    UV_VERSION_MAJOR, UV_VERSION_MINOR, UV_VERSION_PATCH,
                    UV_VERSION_SUFFIX, uv_version_string()));
  }

  // Initialize the event loop
  int res = uv_loop_init(&m_loop);
  if (res < 0) {
    throw std::runtime_error(
        fmt::format("Could not initialize event loop: {}", uv_strerror(res)));
  }

  luaL_openlibs(L);
}

Runtime::~Runtime() { uv_loop_close(&m_loop); }

void Runtime::loadFile(const char *, const char *, const char *) {
  // Fixme: Implement this! It is overridden by EngineImpl, as it uses
  // SDL_rwops, so that loading from the filesystem is always uniform with
  // other LEGE functions. We can't use SDL here as the RT could be used in a
  // CLI environment.
  // Todo: investigate using mmap() here
  throw std::runtime_error("Runtime::loadFile() is Unimplemented!");
}

void Runtime::load(const char *buf, std::size_t size, const char *mode,
                   const char *name) {
  // If we're loading the main chunk, put it in the registry instead of
  // package.preload
  if (std::strcmp(name, "main") == 0) {
    lua_pushvalue(L, LUA_REGISTRYINDEX);
  } else {
    // Get the package.preload table
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "preload");
    lua_replace(L, -2); // Replace package with package.preload
  }

  // Load the buffer as a chunk
  L.load(buf, size, mode, name);

  // package.preload[name] = chunk
  // or registry.main = chunk if this is the main chunk
  lua_setfield(L, -2, name);

  // Pop package.preload / the registry
  lua_pop(L, 1);
}

void Runtime::load(lua_CFunction cfunc, std::string_view name) {
  // Get the package.preload table
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  lua_replace(L, -2); // Replace package with package.preload
  lua::push(L, name);
  lua_pushcfunction(L, cfunc);
  lua_rawset(L, -3);
  lua_pop(L, 1);
}

void Runtime::setup() {
  lua_getfield(L, LUA_REGISTRYINDEX, "main");
  if (lua_type(L, -1) != LUA_TFUNCTION) {
    throw std::runtime_error("Main chunk not loaded");
  }
  lua_call(L, 0, 0);
}

bool Runtime::runOnce() {
  // Run the libuv event loop
  int res = uv_run(&m_loop, UV_RUN_NOWAIT);
  if (res < 0) {
    throw std::runtime_error(
        fmt::format("Error running event loop: {}", uv_strerror(res)));
  }

  lua_settop(L, 0); // Clear stack

  // Run pending tasks
  luaL_getmetatable(L, LEGE_TASK_ENV_NAME);
  lua_pushliteral(L, "dead");
  lua_rawget(L, 1);
  lua_pushliteral(L, "pending");
  lua_rawget(L, 1);

  // Loop over pending tasks and run them
  std::size_t numAliveTasks = 0;
  for (lua_pushnil(L); lua_next(L, 3); lua_settop(L, 4)) {
    // 1 = env, 2 = dead, 3 = pending, 4 = task
    ++numAliveTasks;
    lua_settop(L, 4); // Pop `true' value

    lua_getfenv(L, 4);
    lua_pushliteral(L, "co");
    lua_rawget(L, -2);
    lua_State *co = lua_tothread(L, -1);
    switch (lua_resume(co, 0)) {
    case LUA_OK:
      // Coroutine finished, is now dead

      // dead[task] = true
      lua_pushvalue(L, 4);
      lua_pushboolean(L, true);
      lua_rawset(L, 2);
      break;
    case LUA_YIELD:
      // Coroutine yielded, is now either blocked or still pending
      break;
    default:
      throw lua::Error(co, "Error running coroutine");
    }
  }

  // Remove dead tasks
  for (lua_pushnil(L); lua_next(L, 2); lua_settop(L, 4)) {
    // 1 = env, 2 = dead, 3 = pending, 4 = task

    // pending[task] = nil
    lua_pushvalue(L, 4);
    lua_pushnil(L);
    lua_rawset(L, 3);
    // dead is automatically cleaned up because it's a weak-keyed table
  }

  lua_settop(L, 1);
  lua_pushliteral(L, "blocked");
  lua_rawget(L, 1);
  // Count the blocked tasks
  // Todo: cache this, update the cache when tasks are blocked / scheduled
  for (lua_pushnil(L); lua_next(L, 2); lua_settop(L, 3)) {
    ++numAliveTasks;
  }

  return numAliveTasks > 0;
}

} // namespace lege
