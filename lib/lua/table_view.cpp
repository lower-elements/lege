#include "lua/table_view.hpp"

namespace lege::lua {

TableView::TableView(lua_State *state, int index) : L(state), m_index(index) {}

TableView TableView::globals(lua_State *L) {
  return TableView(L, LUA_GLOBALSINDEX);
}

TableView TableView::registry(lua_State *L) {
  return TableView(L, LUA_REGISTRYINDEX);
}

} // namespace lege::lua
