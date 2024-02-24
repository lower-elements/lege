#include <exception>
#include <string_view>

#include <SDL.h>
#include <fmt/core.h>
#include <lua.hpp>

#include "engine.hpp"
#include "lege.hpp"
#include "lua/helpers.hpp"

namespace lua = lege::lua;

namespace lege {

Engine::Engine() : m_impl(new EngineImpl) {}

Engine::~Engine() { delete m_impl; }

void Engine::set(std::string_view option, std::string_view val) {
  m_impl->set(option, val);
}

void Engine::load(const char *buf, std::size_t size, const char *mode,
                  const char *name) {
  m_impl->load(buf, size, mode, name);
}

void Engine::setup() { m_impl->setup(); }

bool Engine::runOnce() { return m_impl->runOnce(); }

void Engine::run() {
  m_impl->setup();
  while (m_impl->runOnce()) {
  }
}

EngineImpl::EngineImpl()
    : L(luaL_newstate()), m_sdl_subsystems(SDL_INIT_VIDEO) {
  if (!L) {
    throw std::runtime_error("Could not initialize Lua state");
  }
  lua_atpanic(L, lua::on_error);
  luaL_openlibs(L);
  if (SDL_InitSubSystem(m_sdl_subsystems) < 0) {
    throw std::runtime_error(
        fmt::format("Could not initialize SDL: {}", SDL_GetError()));
  }
}

EngineImpl::~EngineImpl() {
  lua_close(L);
  SDL_QuitSubSystem(m_sdl_subsystems);
}

void EngineImpl::set(std::string_view option, std::string_view val) {
  // Get the options table
  luaL_newmetatable(L, "lege.options");
  lua_pushlstring(L, option.data(), option.size());
  lua_pushlstring(L, val.data(), val.size());
  lua_rawset(L, -3);
  lua_pop(L, 1);
}

void EngineImpl::load(const char *buf, std::size_t size, const char *mode,
                      const char *name) {
  // If we're loading the main chunk, put it in the registry instead of
  // package.preload
  if (SDL_strcmp(name, "main") == 0) {
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
    throw lua::Error(L, "Could not load chunk");
  }
  // package.preload[name] = chunk
  // or registry.main = chunk if this is the main chunk
  lua_setfield(L, -2, name);

  // Pop package.preload / the registry
  lua_pop(L, 1);
}

void EngineImpl::setup() {
  lua_getfield(L, LUA_REGISTRYINDEX, "main");
  if (lua_type(L, -1) != LUA_TFUNCTION) {
    throw std::runtime_error("Main chunk not loaded");
  }
  lua_call(L, 0, 0);
}

bool EngineImpl::runOnce() { return false; }

} // namespace lege
