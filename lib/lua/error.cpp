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

} // namespace lege::lua
