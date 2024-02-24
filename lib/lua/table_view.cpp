#include "lua/table_view.hpp"

namespace lege::lua {

TableView::TableView(lua_State *state, int index) : L(state), m_index(index) {}

TableView TableView::globals(lua_State *L) {
  return TableView(L, LUA_GLOBALSINDEX);
}

TableView TableView::registry(lua_State *L) {
  return TableView(L, LUA_REGISTRYINDEX);
}

template <> void TableValProxy<lua_Number>::pushKey() {
  lua_pushnumber(L, key);
}

template <> void TableValProxy<bool>::pushKey() { lua_pushboolean(L, key); }

template <> void TableValProxy<std::string_view>::pushKey() {
  lua_pushlstring(L, key.data(), key.size());
}

template <> void TableValProxy<const char *>::pushKey() {
  lua_pushstring(L, key);
}

template <> void TableValProxy<lua_CFunction>::pushKey() {
  lua_pushcfunction(L, key);
}

template <> void TableValProxy<void *>::pushKey() {
  lua_pushlightuserdata(L, key);
}

template <> void TableValProxy<int>::pushKey() { lua_pushvalue(L, key); }

} // namespace lege::lua
