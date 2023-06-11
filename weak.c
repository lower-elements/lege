#include <lauxlib.h>
#include <lua.h>
#include <stddef.h>

#include "modules/weak.h"

/**
 * Create tables with weak references to their keys and / or values.
 * This module provides a convenience function for creating weak tables, which
 * are described in [section 2.10.2 of the Lua 5.1 reference
 * manual][luaref-weak-tables].
 *
 * [luaref-weak-tables]: <https://www.lua.org/manual/5.1/manual.html#2.10.2>
 * @usage
 * local weak = require "lege.weak"
 *
 * -- Construct a table with both weak keys and weak values ('kv')
 * local t = weak 'kv' {}
 * print(t) --> weak table kv: 0xdeadbeaf
 * print(getmetatable(t).__mode) --> kv
 * @module lege.weak
 * @return A function, @{weak}, for constructing weak tables
 */

/**
 * Construct a weak table.
 * This function is returned from @{weak}, and allows specifying the table to
 * make weak, according to the mode specified by the first parameter to @{weak}.
 * @function weak_constructor
 * @tparam table tbl The table to make weak
 * @treturn table The passed parameter, now a weak table
 */
static int l_weak_constructor(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_setmetatable(L, 1);
  lua_settop(L, 1);
  return 1;
}

/**
 * Create a weak table.
 * The mode parameter can be one of:
 *
 * - 'k': Table has weak keys
 * - 'v': Table has weak values
 * - 'kv': Table has both weak keys and weak values
 *   @function weak
 *   @tparam string mode The weak mode
 *   @return A function, @{weak_constructor}, for specifying the table to make
 * weak
 *   @raise If the specified mode is invalid
 */
static int l_weak(lua_State *L) {
  const char *mode = lua_tostring(L, 1);
  // Do the lookup and return a closure to create the table
  lua_pushliteral(L, WEAK_MT_PREFIX);
  lua_pushvalue(L, 1);
  lua_concat(L, 2);
  lua_rawget(L, LUA_REGISTRYINDEX);
  if (!lua_istable(L, -1)) {
    return luaL_error(L, "Unknown mode '%s' for weak table", mode);
  }
  // Create the closure
  lua_pushcclosure(L, l_weak_constructor, 1);
  return 1;
}

/**
 * Convert a weak table to a string.
 * The returned string includes the table's mode and address.
 * @meta __tostring
 * @tparam table weak_tbl The weak table to convert to a string
 * @return A string "weak table &lt;mode&gt;: &lt;address&gt;"
 */
static int l_weak_tostring(lua_State *L) {
  lua_pushvalue(L, lua_upvalueindex(1));
  const char *mode = lua_tostring(L, -1);
  const void *ptr = lua_topointer(L, 1);
  lua_pushfstring(L, "weak table %s: %p", mode, ptr);
  return 1;
}

#define make_mt(mode)                                                          \
  if (luaL_newmetatable(L, WEAK_MT_PREFIX #mode)) {                            \
    lua_pushliteral(L, #mode);                                                 \
    lua_setfield(L, -2, "__mode");                                             \
    lua_pushliteral(L, "weak table " #mode);                                   \
    lua_setfield(L, -2, "__name");                                             \
    lua_pushliteral(L, #mode);                                                 \
    lua_pushcclosure(L, l_weak_tostring, 1);                                   \
    lua_setfield(L, -2, "__tostring");                                         \
  }

void ll_require_weak(lua_State *L) {
  // Register metatables
  make_mt(k);
  make_mt(kv);
  make_mt(v);
}

int luaopen_lege_weak(lua_State *L) {
  ll_require_weak(L);
  lua_pushcfunction(L, l_weak);
  return 1;
}
