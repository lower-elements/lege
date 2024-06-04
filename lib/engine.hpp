#ifndef LIBLEGE_ENGINE_HPP
#define LIBLEGE_ENGINE_HPP

#include <string_view>

#include <SDL.h>
#include <lua.hpp>

#include "game_engine.hpp"
#include "runtime.hpp"

namespace lege {

class EngineImpl : public engine::GameEngine, public Runtime {
public:
  EngineImpl();
  ~EngineImpl();

  void set(std::string_view option, std::string_view val);
  std::string get(std::string_view option);

  // Uses SDL_rwops to load files
  void loadFile(const char *filename, const char *mode = "t",
                const char *name = "main");

  void loadProject(const char *projectfile);

  void setup();
  [[nodiscard]] bool runOnce();
  void run();
};

} // namespace lege

#endif
