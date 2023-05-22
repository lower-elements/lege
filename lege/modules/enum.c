#include <SDL_assert.h>
#include <lauxlib.h>
#include <lua.h>

/**
 * Allows the creation of enumerations.
 * Lege enums are immutable, meaning trying to add or change enum values after
 * their creation is an error, and detect accesses to undefined enum values.
 *
 * Values can have any underlying type, but their keys must be strings.
 * @usage
 * local enum = require "lege.enum"
 *
 * -- Define an enum
 * local Color = enum 'Color' {
 * -- Strings with numeric keys define enum values
 * "red",
 * "green",
 * "blue",
 * -- String keys allow you to define a different value
 * dark_orange = "red",
 * -- Values don't have to be strings, but keys do
 * black = 0,
 * }
 *
 * print(Color) --> enum Color: 0xdeadbeaf
 *
 * assert(Color.red == "red")
 * assert(Color.green == "green")
 * assert(Color.blue == "blue")
 * assert(Color.dark_orange == "red")
 * assert(Color.black == 0)
 *
 * -- Trying to access an undefined enum value is an error
 * -- print(Color.white) --> Error: no value 'white' on enum 'Color'
 * -- Likewise trying to set a value is an error
 * -- Color.White = 255 --> Error: attempt to set value 'White' on enum 'Color'
 * @module lege.enum
 * @return A function @{enum} for creating new enumeration types.
 */

/**
 * Raises an error caused by access to an undefined enum value.
 * @function _enum_err_nf
 * @tparam enum self The enum instance
 * @tparam string key The key that was undefined
 * @raise An error indicating which value was undefined, and on which enum
 * type
 */
static int l_enum_err_nf(lua_State *L) {
  const char *key = lua_tostring(L, 2);
  lua_pushvalue(L, lua_upvalueindex(1)); // Enum name
  const char *tname = lua_tostring(L, -1);
  return luaL_error(L, "no value '%s' on enum '%s'", key, tname);
}

/**
 * Raises an error caused by trying to modify an enum value.
 * @meta __newindex
 * @tparam enum self The enum instance
 * @tparam string key The key that was undefined
 * @param value The value that you tried to set
 * @raise An error indicating which value was attempted to be modified, and on
 * which enum type
 */
static int l_enum_newindex(lua_State *L) {
  const char *key = lua_tostring(L, 2);
  lua_pushvalue(L, lua_upvalueindex(1)); // Enum name
  const char *tname = lua_tostring(L, -1);
  return luaL_error(L, "attempt to set value '%s' on enum '%s'", key, tname);
}

/**
 * Convert an enum type to a human-readable string.
 * Currently we merely output the enum's name and address.
 * @todo Possibly implement pretty-printing
 * @meta __tostring
 * @tparam enum self The enum to convert to a string
 * @treturn string A formatted string "enum &lt;name&gt;: &lt;address&gt;"
 */
static int l_enum_tostring(lua_State *L) {
  const char *ptr = lua_topointer(L, 1);
  lua_pushvalue(L, lua_upvalueindex(1));
  const char *tname = lua_tostring(L, -1);
  lua_pushfstring(L, "enum %s: %p", tname, ptr);
  return 1;
}

static int l_enum_normalize(lua_State *L) {
  lua_pushnil(L);
  while (lua_next(L, 1) != 0) {
    // Key = 2, value = 3
    int key_is_integer;
    lua_Integer idx = lua_tointegerx(L, 2, &key_is_integer);
    if (key_is_integer) {
      // Value must be a string
      if (lua_type(L, 3) != LUA_TSTRING) {
        lua_pushvalue(L, lua_upvalueindex(1)); // Enum name
        const char *tname = lua_tostring(L, 4);
        return luaL_error(
            L, "bad value for enum '%s' at index #%d: expected string, got %s",
            tname, (int)idx, luaL_typename(L, 3));
      }
      // It is a string, so set up the enum value
      lua_pushvalue(L, 3);
      lua_pushvalue(L, 3);
      lua_settable(L, 1);
      // Delete the old key-value pair
      lua_pushvalue(L, 2);
      lua_pushnil(L);
      lua_settable(L, 1);
    } else if (lua_type(L, 2) == LUA_TSTRING) {
      // Do nothing: use whatever value is provided
    } else {
      // Invalid type for key
      lua_pushvalue(L, lua_upvalueindex(1)); // Enum name
      const char *tname = lua_tostring(L, 4);
      return luaL_error(
          L, "bad key for enum '%s': expected integer or string, got %s", tname,
          luaL_typename(L, 2));
    }
    lua_settop(L, 2); // Prepare for next iteration
  }
  lua_settop(L, 1);
  return 0;
}

/**
 * Define an enum's values.
 * A closure over an enum's name returned from @{enum} which creates a new
 * metatable for the enum type, and allows you to define what
 * values the struct has.
 * @function enum_body
 * @tparam table values Values for the new struct type
 * @treturn enum The new enum type
 */
static int l_enum_body(lua_State *L) {
  // Upvalue 1 = enum name, arg 1 = enum values table
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1); // Discard any additional values
  // Normalize the values table
  SDL_assert(l_enum_normalize(L) == 0);
  lua_settop(L, 1);
  // Create the metatable for the values table
  lua_createtable(L, 0, 2);
  lua_pushvalue(L, lua_upvalueindex(1)); // Enum name
  lua_pushcclosure(L, l_enum_err_nf, 1);
  lua_pushvalue(L, 3);
  lua_setfield(L, 2, "__index");
  lua_setfield(L, 2, "__newindex");
  // Set the metatable
  lua_setmetatable(L, 1);
  // Create the table that will act as the enum type
  lua_newtable(L);
  // Create and populate the metatable for the enum type
  lua_createtable(L, 0, 4);
  // The enum's name for the interpriter
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_setfield(L, 3, "__name");
  // Converting enums to strings shows their name and address
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushcclosure(L, l_enum_tostring, 1);
  lua_setfield(L, 3, "__tostring");
  // Allow accessing the enum values
  lua_pushvalue(L, 1);
  lua_setfield(L, 3, "__index");
  // Trying to set a value is an error
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushcclosure(L, l_enum_newindex, 1);
  lua_setfield(L, 3, "__newindex");
  // Set the enum's metatable
  lua_setmetatable(L, 2);
  // Return the enum type
  return 1;
}

/**
 * Create an enum type.
 * @function enum
 * @tparam string name The enum's type name (used when converting to string)
 * @return A closure over name @{enum_body} to allow you to specify the
 * enum's values
 */
static int l_enum(lua_State *L) {
  luaL_checktype(L, 1, LUA_TSTRING);
  lua_settop(L, 1); // Discard any additional values
  lua_pushcclosure(L, l_enum_body, 1);
  return 1;
}

int luaopen_lege_enum(lua_State *L) {
  lua_pushcfunction(L, l_enum);
  return 1;
}
