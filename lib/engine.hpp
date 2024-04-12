#ifndef LIBLEGE_ENGINE_HPP
#define LIBLEGE_ENGINE_HPP

#include <string_view>

#include <SDL.h>
#include <lua.hpp>

#include "runtime.hpp"

namespace lege {

class EngineImpl : public Runtime {
public:
  EngineImpl();
  ~EngineImpl();

  void set(std::string_view option, std::string_view val);

  // Uses SDL_rwops to load files
  void loadFile(const char *filename, const char *mode = "t",
                const char *name = "main");

  void loadProject(const char *projectfile);

  void setup();

private:
  Uint32 m_sdl_subsystems;
};

} // namespace lege

#endif
