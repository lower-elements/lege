#include <cstdlib>
#include <exception>

#include <SDL.h>
#include <fmt/core.h>
#include <lua.hpp>

#include "engine.hpp"
#include "lege.hpp"

namespace lege {

Engine::Engine() : m_impl(new EngineImpl) {}

Engine::~Engine() { delete m_impl; }

int Engine::run() { return m_impl->run(); }

EngineImpl::EngineImpl()
    : L(luaL_newstate()), m_sdl_subsystems(SDL_INIT_VIDEO) {
  if (!L) {
    throw std::runtime_error("Could not initialize Lua state");
  }
  if (SDL_InitSubSystem(m_sdl_subsystems) < 0) {
    throw std::runtime_error(
        fmt::format("Could not initialize SDL: {}", SDL_GetError()));
  }
}

EngineImpl::~EngineImpl() {
  lua_close(L);
  SDL_QuitSubSystem(m_sdl_subsystems);
}

int EngineImpl::run() { return EXIT_SUCCESS; }

} // namespace lege
