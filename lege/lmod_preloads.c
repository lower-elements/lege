#include <lauxlib.h>
#include <lua.h>
#include <string.h>

// Include Lua module bytecode
// Nothing here yet

struct lmod {
  const char *name;
  const unsigned char *buf;
  size_t size;
};

#define lmod(name)                                                             \
  { "lege." #name, luaJIT_BC_##name, luaJIT_BC_##name##_SIZE }

static const struct lmod LUA_MODS[] = {{NULL, NULL, 0}};

static int load_lmod(lua_State *L) {
  // Upvalue 1 is a pointer to the module info
  lua_pushvalue(L, lua_upvalueindex(1));
  const struct lmod *lmod = lua_touserdata(L, -1);
  // Check the module name matches
  const char *modname = lua_tostring(L, 1);
  if (strcmp(modname, lmod->name) != 0) {
    return luaL_error(L, "Failed to load package %s", modname);
  }
  // Load the module from the bytecode
  if (luaL_loadbufferx(L, (const char *)lmod->buf, lmod->size, lmod->name,
                       "b") != LUA_OK) {
    return lua_error(L); // Error already on stack
  }
  // Else call the loaded chunk
  lua_call(L, 0, 1);
  return 1;
}

void lege_preload_lmods(lua_State *L) {
  // On entry to this function, package.preload is at -1
  for (const struct lmod *lmod = LUA_MODS; lmod->name; lmod++) {
    lua_pushlightuserdata(L, (void *)lmod);
    lua_pushcclosure(L, load_lmod, 1);
    lua_setfield(L, -2, lmod->name);
  }
}
