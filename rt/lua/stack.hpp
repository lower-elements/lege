#ifndef LIBLEGE_LUA_STAK_HPP
#define LIBLEGE_LUA_STAK_HPP

#include <optional>
#include <string>
#include <string_view>

#include <lua.hpp>

#include "lua/nil.hpp"

namespace lege::lua {

class TableView;

static inline int absindex(lua_State *L, int index) {
  return index >= 0 ? index : lua_gettop(L) + index + 1;
}

static inline lua_Number &get(lua_State *L, int index, lua_Number &val) {
  val = lua_tonumber(L, index);
  return val;
}

static inline bool &get(lua_State *L, int index, bool &val) {
  val = static_cast<bool>(lua_toboolean(L, index));
  return val;
}

static inline std::string_view &get(lua_State *L, int index,
                                    std::string_view &val) {
  std::size_t len;
  const char *str = lua_tolstring(L, index, &len);
  val = std::string_view(str, len);
  return val;
}

static inline std::string &get(lua_State *L, int index, std::string &val) {
  std::size_t len;
  const char *str = lua_tolstring(L, index, &len);
  val.assign(str, len);
  return val;
}

static inline const char *&get(lua_State *L, int index, const char *&val) {
  val = lua_tostring(L, index);
  return val;
}

static inline lua_CFunction &get(lua_State *L, int index, lua_CFunction &val) {
  val = lua_tocfunction(L, index);
  return val;
}

static inline void *&get(lua_State *L, int index, void *&val) {
  val = lua_touserdata(L, index);
  return val;
}

static inline lua_Integer &get(lua_State *L, int index, lua_Integer &val) {
  val = lua_tointeger(L, index);
  return val;
}

static inline lua_State *&get(lua_State *L, int index, lua_State *&val) {
  val = lua_tothread(L, index);
  return val;
}

TableView &get(lua_State *L, int index, TableView &val);

static inline lua_Number &arg(lua_State *L, int index, lua_Number &val) {
  val = luaL_checknumber(L, index);
  return val;
}

#ifdef LUAJIT_VERSION
template <class T> T *get_cdata(lua_State *L, int index) {
  // 10 = cdata:
  // https://luajit.freelists.narkive.com/rUtQhJVZ/accessing-ffi-cdata-from-the-lua-c-api
  if (lua_type(L, index) != 10) {
    return nullptr;
  }
  return (T *)lua_topointer(L, index);
}
#endif

static inline bool &arg(lua_State *L, int index, bool &val) {
  luaL_checktype(L, index, LUA_TBOOLEAN);
  val = static_cast<bool>(lua_toboolean(L, index));
  return val;
}

static inline std::string_view &arg(lua_State *L, int index,
                                    std::string_view &val) {
  std::size_t len;
  const char *str = luaL_checklstring(L, index, &len);
  val = std::string_view(str, len);
  return val;
}

static inline std::string &arg(lua_State *L, int index, std::string &val) {
  std::size_t len;
  const char *str = luaL_checklstring(L, index, &len);
  val.assign(str, len);
  return val;
}

static inline const char *&arg(lua_State *L, int index, const char *&val) {
  val = luaL_checkstring(L, index);
  return val;
}

static inline lua_CFunction &arg(lua_State *L, int index, lua_CFunction &val) {
  val = lua_tocfunction(L, index);
  luaL_argcheck(L, val != nullptr, index, "`CFunction` expected");
  return val;
}

static inline void *&arg(lua_State *L, int index, void *&val, const char *mt) {
  val = luaL_checkudata(L, index, mt);
  return val;
}

static inline lua_Integer &arg(lua_State *L, int index, lua_Integer &val) {
  val = luaL_checkinteger(L, index);
  return val;
}

static inline lua_State *&arg(lua_State *L, int index, lua_State *&val) {
  luaL_checktype(L, index, LUA_TTHREAD);
  val = lua_tothread(L, index);
  return val;
}

TableView &arg(lua_State *L, int index, TableView &val);

#ifdef LUAJIT_VERSION
template <class T> T &arg_cdata(lua_State *L, int index) {
  if (lua_type(L, index) != 10) {
    luaL_typerror(L, index, "cdata");
  }
  return *(T *)lua_topointer(L, index);
}
#endif

static inline lua_Number &opt_arg(lua_State *L, int index, lua_Number &val,
                                  lua_Number def) {
  val = luaL_optnumber(L, index, def);
  return val;
}

static inline bool &opt_arg(lua_State *L, int index, bool &val, bool def) {
  if (lua_isnil(L, index)) {
    val = def;
    return val;
    ;
  }
  luaL_checktype(L, index, LUA_TBOOLEAN);
  val = static_cast<bool>(lua_toboolean(L, index));
  return val;
}

static inline std::string_view &
opt_arg(lua_State *L, int index, std::string_view &val, std::string_view def) {
  std::size_t len = def.size();
  const char *str = luaL_optlstring(L, index, def.data(), &len);
  val = std::string_view(str, len);
  return val;
}

static inline std::string &opt_arg(lua_State *L, int index, std::string &val,
                                   std::string def) {
  std::size_t len;
  const char *str = luaL_optlstring(L, index, def.c_str(), &len);
  if (str == def.c_str()) {
    val = def;
  } else {
    val.assign(str, len);
  }
  return val;
}

static inline const char *&opt_arg(lua_State *L, int index, const char *&val,
                                   const char *def) {
  val = luaL_optstring(L, index, def);
  return val;
}

static inline lua_CFunction &opt_arg(lua_State *L, int index,
                                     lua_CFunction &val, lua_CFunction def) {
  if (lua_isnil(L, index)) {
    val = def;
    return val;
  }
  val = lua_tocfunction(L, index);
  luaL_argcheck(L, val != nullptr, index, "`CFunction` expected");
  return val;
}

static inline void *&opt_arg(lua_State *L, int index, void *&val,
                             const char *mt, void *def) {
  if (lua_isnil(L, index)) {
    val = def;
    return val;
  }
  val = luaL_checkudata(L, index, mt);
  return val;
}

static inline lua_Integer &opt_arg(lua_State *L, int index, lua_Integer &val,
                                   lua_Integer def) {
  val = luaL_optinteger(L, index, def);
  return val;
}

static inline lua_State *&opt_arg(lua_State *L, int index, lua_State *&val,
                                  lua_State *def) {
  if (lua_isnil(L, index)) {
    val = def;
    return val;
  }
  luaL_checktype(L, index, LUA_TTHREAD);
  val = lua_tothread(L, index);
  return val;
}

std::optional<TableView> &opt_arg(lua_State *L, int index,
                                  std::optional<TableView> &val);

static inline void push(lua_State *L, Nil) { lua_pushnil(L); }

static inline void push(lua_State *L, lua_Number val) {
  lua_pushnumber(L, val);
}

#ifdef LUAJIT_VERSION
template <class T> T &opt_arg_cdata(lua_State *L, int index, T &def) {
  if (lua_isnoneornil(L, index)) {
    return def;
  } else if (lua_type(L, index) != 10) {
    luaL_typerror(L, index, "cdata");
  }
  return *(T *)lua_topointer(L, index);
}
#endif

static inline void push(lua_State *L, bool val) { lua_pushboolean(L, val); }

static inline void push(lua_State *L, std::string_view val) {
  lua_pushlstring(L, val.data(), val.size());
}

static inline void push(lua_State *L, const std::string &val) {
  lua_pushlstring(L, val.data(), val.size());
}

static inline void push(lua_State *L, const char *val) {
  lua_pushstring(L, val);
}

static inline void push(lua_State *L, lua_CFunction val, int nups) {
  lua_pushcclosure(L, val, nups);
}

static inline void push(lua_State *L, void *val) {
  lua_pushlightuserdata(L, val);
}

static inline void push(lua_State *L, lua_Integer val) {
  lua_pushinteger(L, val);
}

} // namespace lege::lua

#endif
