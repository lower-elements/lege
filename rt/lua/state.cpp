#include <cstring>

#include <fmt/core.h>

#include "lua/error.hpp"
#include "lua/state.hpp"

namespace lege::lua {

State::State() : L(luaL_newstate()) {
  if (!L) {
    throw std::runtime_error("Could not initialize Lua state");
  }
}

State::~State() {
  if (L != nullptr) {
    lua_close(L);
  }
}

void State::load(const char *buf, std::size_t size, const char *mode,
                 const char *name) {
  // Load the buffer as a chunk
  int res = luaL_loadbufferx(L, buf, size, name, mode);
  if (res != LUA_OK) {
    throw lua::Error(L, fmt::format("Could not load chunk \"{}\"", name));
  }
}

} // namespace lege::lua
