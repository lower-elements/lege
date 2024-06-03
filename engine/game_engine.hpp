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

private:
  Uint32 m_sdl_subsystems;
};

} // namespace lege::engine

#endif
