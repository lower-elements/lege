#include "game_engine.hpp"

#include "sdl/helpers.hpp"

namespace sdl = lege::sdl;

namespace lege::engine {

GameEngine::GameEngine() {
  setSDLLogPriority();
  initSdlSubSystem(SDL_INIT_VIDEO);
  m_win = SDL_CreateWindow("Loading...", SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED, 640, 480,
                           SDL_WINDOW_HIDDEN | SDL_WINDOW_ALLOW_HIGHDPI);
  if (m_win == nullptr) {
    throw sdl::Error("Could not create window");
  }
}

GameEngine::~GameEngine() {
  if (m_win) {
    SDL_DestroyWindow(m_win);
  }
  SDL_QuitSubSystem(m_sdl_subsystems);
}

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

void GameEngine::setup() {}

bool GameEngine::runOnce() {
  // Process SDL events
  SDL_Event e;
  for (SDL_Event e; SDL_PollEvent(&e);) {
    switch (e.type) {
    case SDL_QUIT:
      return false; // Done
    }
  }
  return true; // Not done
}

} // namespace lege::engine
