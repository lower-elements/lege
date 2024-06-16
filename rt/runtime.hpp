#ifndef LIBLEGE_RUNTIME_HPP
#define LIBLEGE_RUNTIME_HPP

#include <string_view>

#include <uv.h>

#include "lua/state.hpp"

namespace lege {

class Runtime {
public:
  Runtime();
  ~Runtime();

  void load(const char *buf, std::size_t size, const char *mode,
            const char *name);
  void load(lua_CFunction cfunc, std::string_view name);
  void loadFile(const char *filename, const char *mode = "t",
                const char *name = "main");

  void setup();
  bool runOnce();

protected:
  uv_loop_t m_loop;
  lua::State L;
};

} // namespace lege

#endif
