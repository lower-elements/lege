#include "game_engine.hpp"

#include "sdl/helpers.hpp"

namespace sdl = lege::sdl;

namespace lege::engine {

GameEngine::GameEngine() {
  setSDLLogPriority();
  initSdlSubSystem(SDL_INIT_VIDEO);
}

GameEngine::~GameEngine() { SDL_QuitSubSystem(m_sdl_subsystems); }

void GameEngine::initSdlSubSystem(Uint32 subsystems) {
  if (SDL_InitSubSystem(subsystems) < 0) {
    throw sdl::Error("Could not initialize SDL subsystems");
  }
  m_sdl_subsystems |= subsystems;
}

void GameEngine::quitSdlSubSystem(Uint32 subsystems) {
  SDL_QuitSubSystem(subsystems);
  m_sdl_subsystems &= ~subsystems;
}

} // namespace lege::engine
