#ifndef LIBLEGE_LUA_STATE_HPP
#define LIBLEGE_LUA_STATE_HPP

#include <stdexcept>

#include <lua.hpp>

namespace lege::lua {

class State {
public:
  State(lua_State *state) : L(state) {}
  State() : L(luaL_newstate()) {
    if (!L) {
      throw std::runtime_error("Could not initialize Lua state");
    }
  }

  // No copy
  State(const State &) = delete;
  State &operator=(const State &) = delete;

  State(State &&s) : L(s.L) { s.L = nullptr; }
  State &operator=(State &&s) {
    L = s.L;
    s.L = nullptr;
    return *this;
  }

  ~State() {
    if (L != nullptr) {
      lua_close(L);
    }
  }

  lua_State *get() & { return L; }

  operator lua_State *() & { return get(); }

private:
  lua_State *L;
};

} // namespace lege::lua

#endif
