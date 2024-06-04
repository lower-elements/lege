#ifndef LIBLEGE_ENGINE_GAME_ENGINE_HPP
#define LIBLEGE_ENGINE_GAME_ENGINE_HPP

#include <SDL.h>

namespace lege::engine {

class GameEngine {
public:
  GameEngine();
  ~GameEngine();

  void initSdlSubSystem(Uint32 subsystems);
  void quitSdlSubSystem(Uint32 subsystems);

  SDL_Window *getWindow() { return m_win; }

  void setup();
  [[nodiscard]] bool runOnce();

private:
  SDL_Window *m_win = nullptr;
  Uint32 m_sdl_subsystems = 0;
};

} // namespace lege::engine

#endif
