#include <lauxlib.h>
#include <lua.h>

#include "preloads.h"

static const luaL_Reg BUILTIN_PRELOADS[] = {
    {.name = "lege.log", .func = luaopen_lege_log},
    {.name = "lege.readonly", .func = luaopen_lege_readonly},
    {.name = "lege.strict", .func = luaopen_lege_strict},
    {.name = "lege.struct", .func = luaopen_lege_struct},
    {NULL, NULL}};

static int run_loader(lua_State *L) {
  // Check first argument (the package to load) is a string
  const char *pkg = luaL_checkstring(L, 1);
  // Check it's the same as the name of the package this closure loads
  lua_pushvalue(L, lua_upvalueindex(1));
  if (!lua_rawequal(L, 1, -1)) {
    // Not the same package
    return luaL_error(L, "Failed to load package %s", pkg);
  }

  // Call the loader
  lua_pushvalue(L, lua_upvalueindex(2));
  lua_call(L, 0, 1);
  return 1;
}

void lege_preload_builtins(lua_State *L) {
  // Get the package.preload table
  lua_getglobal(L, "package");
  lua_getfield(L, -1, "preload");
  for (const luaL_Reg *preload = BUILTIN_PRELOADS; preload->name; ++preload) {
    // Push the key for the assignment
    lua_pushstring(L, preload->name);
    // Push it again as the first upvalue to the closure
    lua_pushvalue(L, -1);
    // Push the CFuntion as the second upvalue to the closure
    lua_pushcfunction(L, preload->func);
    // Create the closure
    lua_pushcclosure(L, run_loader, 2);
    // package.preload[name] = closure
    lua_rawset(L, -3);
  }
}
