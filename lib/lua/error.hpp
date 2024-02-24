#ifndef LIBLEGE_LUA_ERROR_HPP
#define LIBLEGE_LUA_ERROR_HPP

#include <exception>
#include <string>

#include <lua.hpp>

namespace lege::lua {

class Error : public std::exception {
public:
  Error(lua_State *L, const char *prefix = "Lua error");
  virtual ~Error() override;

  virtual const char *what() const noexcept override;

private:
  std::string m_msg;
};

int on_error(lua_State *L);

} // namespace lege::lua

#endif
