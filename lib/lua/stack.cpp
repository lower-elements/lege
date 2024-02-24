#include "lua/stack.hpp"
#include "lua/table_view.hpp"

namespace lege::lua {

void get(lua_State *L, int index, lua_Number &val) {
  val = lua_tonumber(L, index);
}

void get(lua_State *L, int index, bool &val) {
  val = static_cast<bool>(lua_toboolean(L, index));
}

void get(lua_State *L, int index, std::string_view &val) {
  std::size_t len;
  const char *str = lua_tolstring(L, index, &len);
  val = std::string_view(str, len);
}

void get(lua_State *L, int index, std::string &val) {
  std::size_t len;
  const char *str = lua_tolstring(L, index, &len);
  val.assign(str, len);
}

void get(lua_State *L, int index, const char *&val) {
  val = lua_tostring(L, index);
}

void get(lua_State *L, int index, lua_CFunction &val) {
  val = lua_tocfunction(L, index);
}

void get(lua_State *L, int index, void *&val) {
  val = lua_touserdata(L, index);
}

void get(lua_State *L, int index, lua_Integer &val) {
  val = lua_tointeger(L, index);
}

void get(lua_State *L, int index, lua_State *&val) {
  val = lua_tothread(L, index);
}

void get(lua_State *L, int index, TableView &val) { val = TableView(L, index); }

void arg(lua_State *L, int index, lua_Number &val) {
  val = luaL_checknumber(L, index);
}

void arg(lua_State *L, int index, bool &val) {
  luaL_checktype(L, index, LUA_TBOOLEAN);
  val = static_cast<bool>(lua_toboolean(L, index));
}

void arg(lua_State *L, int index, std::string_view &val) {
  std::size_t len;
  const char *str = luaL_checklstring(L, index, &len);
  val = std::string_view(str, len);
}

void arg(lua_State *L, int index, std::string &val) {
  std::size_t len;
  const char *str = luaL_checklstring(L, index, &len);
  val.assign(str, len);
}

void arg(lua_State *L, int index, const char *&val) {
  val = luaL_checkstring(L, index);
}

void arg(lua_State *L, int index, lua_CFunction &val) {
  val = lua_tocfunction(L, index);
  luaL_argcheck(L, val != nullptr, index, "`CFunction` expected");
}

void arg(lua_State *L, int index, void *&val, const char *mt) {
  val = luaL_checkudata(L, index, mt);
}

void arg(lua_State *L, int index, lua_Integer &val) {
  val = luaL_checkinteger(L, index);
}

void arg(lua_State *L, int index, lua_State *&val) {
  luaL_checktype(L, index, LUA_TTHREAD);
  val = lua_tothread(L, index);
}

void arg(lua_State *L, int index, TableView &val) {
  luaL_checktype(L, index, LUA_TTABLE);
  val = TableView(L, index);
}

void opt_arg(lua_State *L, int index, lua_Number &val, lua_Number def) {
  val = luaL_optnumber(L, index, def);
}

void opt_arg(lua_State *L, int index, bool &val, bool def) {
  if (lua_isnil(L, index)) {
    val = def;
    return;
  }
  luaL_checktype(L, index, LUA_TBOOLEAN);
  val = static_cast<bool>(lua_toboolean(L, index));
}

void opt_arg(lua_State *L, int index, std::string_view &val,
             std::string_view def) {
  std::size_t len = def.size();
  const char *str = luaL_optlstring(L, index, def.data(), &len);
  val = std::string_view(str, len);
}

void opt_arg(lua_State *L, int index, std::string &val, std::string def) {
  std::size_t len;
  const char *str = luaL_optlstring(L, index, def.c_str(), &len);
  if (str == def.c_str()) {
    val = def;
  } else {
    val.assign(str, len);
  }
}

void opt_arg(lua_State *L, int index, const char *&val, const char *def) {
  val = luaL_optstring(L, index, def);
}

void opt_arg(lua_State *L, int index, lua_CFunction &val, lua_CFunction def) {
  if (lua_isnil(L, index)) {
    val = def;
    return;
  }
  val = lua_tocfunction(L, index);
  luaL_argcheck(L, val != nullptr, index, "`CFunction` expected");
}

void opt_arg(lua_State *L, int index, void *&val, const char *mt, void *def) {
  if (lua_isnil(L, index)) {
    val = def;
    return;
  }
  val = luaL_checkudata(L, index, mt);
}

void opt_arg(lua_State *L, int index, lua_Integer &val, lua_Integer def) {
  val = luaL_optinteger(L, index, def);
}

void opt_arg(lua_State *L, int index, lua_State *&val, lua_State *def) {
  if (lua_isnil(L, index)) {
    val = def;
    return;
  }
  luaL_checktype(L, index, LUA_TTHREAD);
  val = lua_tothread(L, index);
}

void opt_arg(lua_State *L, int index, std::optional<TableView> &val) {
  if (lua_isnil(L, index)) {
    val = {};
    return;
  }
  luaL_checktype(L, index, LUA_TTABLE);
  val = TableView(L, index);
}

void push(lua_State *L, Nil) { lua_pushnil(L); }

void push(lua_State *L, lua_Number val) { lua_pushnumber(L, val); }

void push(lua_State *L, bool val) { lua_pushboolean(L, val); }

void push(lua_State *L, std::string_view val) {
  lua_pushlstring(L, val.data(), val.size());
}

void push(lua_State *L, const std::string &val) {
  lua_pushlstring(L, val.data(), val.size());
}

void push(lua_State *L, const char *val) { lua_pushstring(L, val); }

void push(lua_State *L, lua_CFunction val, int nups) {
  lua_pushcclosure(L, val, nups);
}

void push(lua_State *L, void *val) { lua_pushlightuserdata(L, val); }

void push(lua_State *L, lua_Integer val) { lua_pushinteger(L, val); }

} // namespace lege::lua
