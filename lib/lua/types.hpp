#ifndef LIBLEGE_LUA_TYPES_HPP
#define LIBLEGE_LUA_TYPES_HPP

#include <cstddef>
#include <type_traits>
#include <utility>

#include <lua.hpp>

#include "lua/table_view.hpp"
#include "util.hpp"

namespace lege::lua {

// We assume we'll get something at least 16 byte aligned since LuaJIT seems to
// align to 16 bytes
// Todo: Determine this at configure-time?
inline constexpr std::size_t MIN_USERDATA_ALIGNMENT = 16;

template <class T> constexpr std::string get_mt_name() {
  return fmt::format("mt.{}", lege::type_id<T>());
}

template <class T> void make_metatable(lua_State *L) {
  // Each C++ type has its own distinct metatable. We ensure uniqueness by
  // getting a unique hash for the specific type we're creating a metatable for
  std::string mt_name(get_mt_name<T>());
  if (!luaL_newmetatable(L, mt_name.c_str())) {
    return; // mt already exists
  }

  // Construct the new metatable
  // Set mt.__name to a human readable type signature for the C++ type
  lua_pushliteral(L, "__name");
  auto name = lege::demangled_name<T>();
  lua_pushstring(L, name.get());
  lua_rawset(L, -3);

  if constexpr (std::is_destructible_v<T>) {
    // Call the destructor upon garbage collection
    lua_pushliteral(L, "__gc");
    lua_pushcfunction(L, [](lua_State *L) {
      T *ptr = static_cast<T *>(lua_touserdata(L, 1));
      ptr->~T();
      return 0;
    });
    lua_rawset(L, -3);
  }
  // Todo: Add more metatable fields automatically (E.G. __add for types that
  // implement operator+(), etc)
}

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

  // Get and / or create metatable
  make_metatable<T>(L);
  lua_setmetatable(L, -2);

  return new (ptr) T(std::forward<Args...>(args...));
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
  return new (ptr) T(std::forward<Args...>(args...));
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
  return new (ptr) T(std::forward<Args...>(args...));
}

static inline TableView new_table(lua_State *L, int narr = 0, int nrec = 0) {
  lua_createtable(L, narr, nrec);
  return TableView(L, lua_gettop(L));
}

} // namespace lege::lua

#endif
