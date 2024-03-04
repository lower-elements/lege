#include "lua/stack.hpp"
#include "lua/table_view.hpp"

namespace lege::lua {

// These have to be here otherwise we'd have a circular reference to TableView
TableView &get(lua_State *L, int index, TableView &val) {
  val = TableView(L, index);
  return val;
}

TableView &arg(lua_State *L, int index, TableView &val) {
  luaL_checktype(L, index, LUA_TTABLE);
  val = TableView(L, index);
  return val;
}

std::optional<TableView> &opt_arg(lua_State *L, int index,
                                  std::optional<TableView> &val) {
  if (lua_isnil(L, index)) {
    val = {};
    return val;
  }
  luaL_checktype(L, index, LUA_TTABLE);
  val = TableView(L, index);
  return val;
}

} // namespace lege::lua
