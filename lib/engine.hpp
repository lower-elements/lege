#ifndef LIBLEGE_ENGINE_HPP
#define LIBLEGE_ENGINE_HPP

namespace lege {

class EngineImpl {
public:
  EngineImpl();
  ~EngineImpl();

  void set(std::string_view option, std::string_view val);

  void load(const char *buf, std::size_t size, const char *mode,
            const char *name);

  void setup();
  bool runOnce();

private:
  lua_State *L;
  Uint32 m_sdl_subsystems;
};

} // namespace lege

#endif
