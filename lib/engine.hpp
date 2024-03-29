#ifndef LIBLEGE_ENGINE_HPP
#define LIBLEGE_ENGINE_HPP

#include <string_view>

#include <SDL.h>
#include <lua.hpp>

namespace lege {

class EngineImpl {
public:
  EngineImpl();
  ~EngineImpl();

  void set(std::string_view option, std::string_view val);

  void load(const char *buf, std::size_t size, const char *mode,
            const char *name);
  void load(lua_CFunction cfunc, std::string_view name);
  void loadFile(const char *filename, const char *mode = "t",
                const char *name = "main");

  void loadProject(const char *projectfile);

  void setup();
  bool runOnce();

private:
  lua_State *L;
  Uint32 m_sdl_subsystems;
};

} // namespace lege

#endif
