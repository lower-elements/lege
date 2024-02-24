#include <cstddef>

#include <fmt/core.h>

#include "lua/error.hpp"

namespace lege::lua {

Error::Error(lua_State *L, const char *prefix) {
  std::size_t errlen;
  const char *errmsg = lua_tolstring(L, -1, &errlen);
  m_msg = fmt::format("{}: {}", prefix, std::string_view(errmsg, errlen));
}

Error::~Error() {}

const char *Error::what() const noexcept { return m_msg.c_str(); }

int on_error(lua_State *L) {
  const char *msg = lua_tostring(L, -1);
  luaL_traceback(L, L, msg, 0);
  lua_replace(L, -2);
  // From here on, msg is invalid, but we no longer need it
  Error err(L);
  lua_pop(L, 1);
  throw err;
}

} // namespace lege::lua
