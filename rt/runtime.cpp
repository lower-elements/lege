#include <exception>
#include <memory>
#include <string_view>

#include <fmt/core.h>
#include <lua.hpp>

#include "lua/helpers.hpp"
#include "runtime.hpp"

namespace lua = lege::lua;

namespace lege {

Runtime::Runtime() : L() {
  lua_atpanic(L, lua::on_error);
  luaL_openlibs(L);
}

Runtime::~Runtime() {}

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
  int res = luaL_loadbufferx(L, buf, size, name, mode);
  if (res != LUA_OK) {
    throw lua::Error(L, fmt::format("Could not load chunk \"{}\"", name));
  }
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

bool Runtime::runOnce() { return false; }

} // namespace lege
