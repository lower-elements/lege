#ifndef LIBLEGE_LUA_NEW_HPP
#define LIBLEGE_LUA_NEW_HPP

#include <utility>

#include <lua.hpp>

#include "lua/table_view.hpp"

namespace lege::lua {

template <class T, class... Args>
T *new_userdata(lua_State *L, Args &&...args) {
  void *ptr = lua_newuserdata(L, sizeof(T));
  return new (ptr) T(std::forward(args...));
}

template <class T, class... Args>
T *new_userdata(lua_State *L, const char *metatable, Args &&...args) {
  void *ptr = lua_newuserdata(L, sizeof(T));
  luaL_setmetatable(L, metatable);
  return new (ptr) T(std::forward(args...));
}

template <class T, class... Args>
T *new_userdata(lua_State *L, int metatable_index, Args &&...args) {
  void *ptr = lua_newuserdata(L, sizeof(T));
  lua_pushvalue(L, metatable_index);
  lua_setmetatable(L, -2);
  return new (ptr) T(std::forward(args...));
}

static inline TableView new_table(lua_State *L, int narr = 0, int nrec = 0) {
  lua_createtable(L, narr, nrec);
  return TableView(L, lua_gettop(L));
}

} // namespace lege::lua

#endif
