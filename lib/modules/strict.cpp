#include <cstdio>
#include <cstring>
#include <lua.hpp>

#include "preloads.hpp"

/**
 * checks uses of undeclared global variables.
 * All global variables must be 'declared' through a regular assignment
 * (even assigning nil will do) in a main chunk before being used
 * anywhere or assigned to inside a function.
 *
 * This is a C API port of "strict.lua" from the Lua distribution
 * @usage
 * require "lege.strict"
 *
 * -- With strict, accessing global variables from anywhere that aren't first
 * declared is an error
 * -- print("The value of x is " .. tostring(x))
 *  --> Error: variable 'x' is not declared
 *
 * -- You declare global variables anywhere in a main chunk (I.E. not in a
 * function)
 * -- They can be set to anything, even nil!
 * x = nil
 * print("The value of x is " .. tostring(x)) --> The value of x is nil
 * @module lege.strict
 */

static const char *what(lua_State *L) {
  lua_Debug d;
  if (lua_getstack(L, 1, &d) && lua_getinfo(L, "S", &d)) {
    return d.what;
  } else {
    return "C";
  }
}

static int l_newindex(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  // Check if this var was declared
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);
  if (lua_isnoneornil(L, -1)) {
    // Check where this global was set
    // Globals set from a main chunk or from C are exempt
    const char *w = what(L);
    if ((std::strcmp(w, "main") != 0) && (std::strcmp(w, "C") != 0)) {
      lua_pushliteral(L, "assign to undeclared variable '");
      lua_pushvalue(L, 2);
      lua_pushliteral(L, "'");
      return lua_error(L);
    }
    // Otherwise, declare the variable
    // But first, pop nil value
    lua_pop(L, 1);
    lua_pushvalue(L, 2);
    lua_pushboolean(L, true);
    lua_rawset(L, -3);
  }
  // Pop __declared
  lua_pop(L, 1);
  // Variable is declared, so assign it as usual
  lua_rawset(L, 1);
  return 0;
}

static int l_index(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  // Check if this var was declared
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);
  if (lua_isnoneornil(L, -1)) {
    // Check this wasn't accessed from C code
    const char *w = what(L);
    if (std::strcmp(w, "C") != 0) {
      // Called from Lua code, illeagal access to undefined global variable
      lua_pushliteral(L, "variable '");
      lua_pushvalue(L, 2);
      lua_pushliteral(L, "' is not declared");
      lua_concat(L, 3);
      return lua_error(L);
    }
  }
  // Variable *is* declared, return its value as usual
  lua_pop(L, 2);
  lua_rawget(L, 1);
  return 1;
}

extern "C" int luaopen_lege_strict(lua_State *L) {
  // Get or create metatable for _G
  lua_pushvalue(L, LUA_GLOBALSINDEX);
  if (!lua_getmetatable(L, -1)) {
    // Create a new one
    lua_newtable(L);
    lua_setmetatable(L, -2);
    lua_getmetatable(L, -1);
  }
  // // Push the name of __newindex (used later)
  lua_pushliteral(L, "__newindex");
  // Create the __declared table
  lua_newtable(L);
  // Set mt.__declared
  lua_pushliteral(L, "__declared");
  lua_pushvalue(L, -2);
  lua_rawset(L, -5);
  // Create and set __index / __newindex closures
  lua_pushliteral(L, "__index");
  lua_pushvalue(L, -2);
  lua_pushcclosure(L, l_index, 1);
  lua_rawset(L, -5);
  // Set __newindex (from earlier)
  lua_pushcclosure(L, l_newindex, 1);
  lua_rawset(L, -3);
  return 0;
}
