#ifndef LIBLEGE_ENGINE_HPP
#define LIBLEGE_ENGINE_HPP

namespace lege {

class EngineImpl {
public:
  EngineImpl();
  ~EngineImpl();

  int run();

private:
  lua_State *L;
  Uint32 m_sdl_subsystems;
};

} // namespace lege

#endif
