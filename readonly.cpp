#include <lua.hpp>

/**
 * Allows the creation of readonly tables.
 * @usage
 * local readonly = require "lege.readonly"
 *
 * local consts = readonly {1, 2, 3; a=42, b=1337}
 *
 * -- Accessing values works as normal
 * print("The value of consts.b is " .. consts.b)
 * --> The value of b is 1337
 *
 * -- But changing them is an error
 * -- consts.b = 100
 * --> Error: attempt to modify readonly table
 *
 * -- You also can't get or set the table's metatable
 * print("consts' metatable is " .. tostring(getmetatable(consts)))
 * --> consts' metatable is false
 * -- setmetatable(consts, {})
 * --> Error: cannot change a protected metatable
 * @module lege.readonly
 * @return A function @{readonly} for making tables readonly
 */

static int l_err_readonly(lua_State *L) {
  lua_pushliteral(L, "attempt to modify readonly table");
  return lua_error(L);
}

/**
 * Make the passed table readonly.
 * @function readonly
 * @tparam table T The table to make readonly
 * @treturn table A table with readonly access to the keys and values in the
 * passed table
 */
static int l_readonly(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  // Create the proxy table
  lua_newtable(L);
  // Create and set up the metatable
  lua_createtable(L, 0, 4);
  lua_pushvalue(L, 1);
  lua_setfield(L, -2, "__index");
  lua_pushcfunction(L, l_err_readonly);
  lua_setfield(L, -2, "__newindex");
  lua_pushliteral(L, "readonly table");
  lua_setfield(L, -2, "__name");
  lua_pushboolean(L, false);
  lua_setfield(L, -2, "__metatable");
  // Set the metatable
  lua_setmetatable(L, -2);
  return 1;
}

extern "C" int luaopen_lege_readonly(lua_State *L) {
  lua_pushcfunction(L, l_readonly);
  return 1;
}
