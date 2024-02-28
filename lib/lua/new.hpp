#ifndef LIBLEGE_LUA_NEW_HPP
#define LIBLEGE_LUA_NEW_HPP

#include <cstddef>
#include <utility>

#include <lua.hpp>

#include "lua/table_view.hpp"
#include "util.hpp"

namespace lege::lua {

// We assume we'll get something at least 16 byte aligned since LuaJIT seems to
// align to 16 bytes
// Todo: Determine this at configure-time?
inline constexpr std::size_t MIN_USERDATA_ALIGNMENT = 16;

template <class T, class... Args>
T *new_userdata(lua_State *L, Args &&...args) {
  std::size_t alloc_size = sizeof(T);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    alloc_size += MIN_USERDATA_ALIGNMENT;
  }
  void *ptr = lua_newuserdata(L, alloc_size);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    ptr = align_up(ptr, alignof(T));
  }
  return new (ptr) T(std::forward(args...));
}

template <class T, class... Args>
T *new_userdata_mt(lua_State *L, const char *metatable, Args &&...args) {
  std::size_t alloc_size = sizeof(T);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    alloc_size += MIN_USERDATA_ALIGNMENT;
  }
  void *ptr = lua_newuserdata(L, alloc_size);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    ptr = align_up(ptr, alignof(T));
  }
  luaL_setmetatable(L, metatable);
  return new (ptr) T(std::forward(args...));
}

template <class T, class... Args>
T *new_userdata_mt(lua_State *L, int metatable_index, Args &&...args) {
  std::size_t alloc_size = sizeof(T);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    alloc_size += MIN_USERDATA_ALIGNMENT;
  }
  void *ptr = lua_newuserdata(L, alloc_size);
  if constexpr (alignof(T) > MIN_USERDATA_ALIGNMENT) {
    ptr = align_up(ptr, alignof(T));
  }
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
