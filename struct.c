#include <lauxlib.h>
#include <lua.h>

/**
 * Allows the creation of strict structs.
 * [Strict structs][strict-structs] defined by this module have strict field
 * access (I.E. trying to get or set the value of a field that is undefined for
 * a given struct type is an error).
 *
 * [strict-structs]: <https://lua-users.org/wiki/StrictStructs>
 * @usage
 * local struct = require "lege.struct"
 *
 * -- Define a Point struct, to hold x and y coordinates
 * local Point = struct 'Point' {
 * x = 0,
 * y = 0,
 * }
 *
 * -- Create a Point
 * local p = Point {
 * x = 10,
 * y = 20,
 * }
 * print(p) --> struct Point: 0xdeadbeaf
 * assert(p.x == 10)
 * assert(p.y == 20)
 * -- Trying to access or modify an undefined field is an error
 * -- print(p.z) --> Error: no field 'z' on struct Point
 *
 * -- When constructing a new struct, unspecified fields get their default
 * values p = Point {x=1} assert(p.x == 1) assert(p.y == 0)
 *
 * -- Struct fields can be modified as you'd expect, but don't set them to nil!
 * p.y = 42
 * assert(p.y == 42)
 * @module lege.struct
 * @return A function @{struct} that Allows you to define structs
 */

/**
 * Raises an error caused by access to an undefined struct field.
 * @function _struct_err_nf
 * @tparam struct self The struct instance
 * @tparam string key The key that was undefined
 * @raise An error indicating which field was undefined, and on which struct
 * type
 */
static int l_struct_err_nf(lua_State *L) {
  const char *key = lua_tostring(L, 2);
  lua_pushvalue(L, lua_upvalueindex(1));
  const char *tname = lua_tostring(L, -1);
  return luaL_error(L, "no field '%s' on struct '%s'", key, tname);
}

/**
 * Convert a struct type to a human-readable string.
 * Currently we merely output the struct's name and address.
 * @todo Possibly implement pretty-printing
 * @meta __tostring
 * @tparam struct self The struct to convert to a string
 * @treturn string A formatted string "struct &lt;name&gt;: &lt;address&gt;"
 */
static int l_struct_tostring(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1);
  lua_getmetatable(L, 1);
  lua_getfield(L, 2, "__name");
  const char *tname = lua_tostring(L, 3);
  const void *ptr = lua_topointer(L, 1);
  lua_pushfstring(L, "struct %s: %p", tname, ptr);
  return 1;
}

/**
 * Create a new struct type.
 * This is used as a closure returned from @{struct_body}, and creates a new
 * instance of a struct. Any unset fields are set to their default value as
 * defined by the @{struct} type.
 * @function new_instance
 * @tparam table instance Table to use as the struct instance, including any
 * pre-initialized fields
 * @treturn table The passed instance, modified to be a struct of the correct
 * type
 * @raise If instance contains undefined fields for the struct type
 */
static int l_struct_new(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1);                      // Discard any additional arguments
                                         // Set the struct's metatable
  lua_pushvalue(L, lua_upvalueindex(2)); // Metatable
  lua_setmetatable(L, 1);
  // Check that all fields are valid for this struct
  lua_pushvalue(L, lua_upvalueindex(1)); // Fields
  lua_pushnil(L);                        // Initial key
  while (lua_next(L, 1) != 0) {
    lua_copy(L, 3, 4);  // Overwrite instance[key] with a duplicate of key
    lua_gettable(L, 2); // Check if key is in fields table
    if (lua_isnil(L, 4)) {
      // Call l_struct_err_nf, passed as upvalue 3
      lua_pushvalue(L, lua_upvalueindex(3));
      lua_pushvalue(L, 1);
      lua_pushvalue(L, 3);
      lua_call(L, 2, 1);
      return 1;
    }
    lua_settop(L, 3); // Pop fields[key]
  }
  // Now set defaults for any undefined fields
  lua_settop(L, 2);
  lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    lua_pushvalue(L, 3); // Push another copy of key
    lua_rawget(L, 1);    // Check if the passed table has the key
                         // puts(luaL_typename(L, 5);
    if (lua_isnil(L, 5)) {
      lua_pushvalue(L, 3); // key
      lua_pushvalue(L, 4); // fields[key]
      lua_rawset(L, 1);
    }
    lua_settop(L, 3);
  }
  // Return the struct instance
  lua_settop(L, 1);
  return 1;
}

/**
 * Define a struct's body.
 * A closure over a struct's name returned from @{struct} which creates a new
 * metatable for the struct type, and allows you to define what fields and
 * default values the struct has.
 * @function struct_body
 * @tparam table fields Fields and default values for the new struct type
 * @return A closure @{new_instance} over the struct type's name and fields to
 * create a new instance of the defined struct type
 */
static int l_struct_body(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1); // Discard any additional arguments
  // Create metatable
  lua_createtable(L, 0, 4);
  // mt.__name = struct_name
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_setfield(L, 2, "__name");
  // Custom __tostring that print's the struct's name
  lua_pushcfunction(L, l_struct_tostring);
  lua_setfield(L, 2, "__tostring");
  // Trying to get or set a field that doesn't exist is an error
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushcclosure(L, l_struct_err_nf, 1);
  lua_pushvalue(L, -1); // Duplicate for __newindex
  lua_pushvalue(L, -1); // Duplicate for upvalue 3 of l_struct_new
  lua_setfield(L, 2, "__index");
  lua_setfield(L, 2, "__newindex");
  // Return a closure that constructs new structs of this type
  lua_pushcclosure(L, l_struct_new, 3);
  return 1;
}

/**
 * Create a struct type.
 * @function struct
 * @param string name The struct's type name (used when converting to string)
 * @treturn A closure over name @{struct_body} to allow you to specify the
 * struct's fields and default values
 */
static int l_struct(lua_State *L) {
  luaL_checkstring(L, 1);
  lua_settop(L, 1); // Discard any additional arguments
  lua_pushcclosure(L, l_struct_body, 1);
  return 1;
}

int luaopen_lege_struct(lua_State *L) {
  lua_pushcfunction(L, l_struct);
  return 1;
}
