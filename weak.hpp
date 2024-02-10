#ifndef LIBLEGE_MOD_WEAK_H
#define LIBLEGE_MOD_WEAK_H

#include <lua.h>

#define WEAK_MT_PREFIX "lege.weak."
#define WEAK_MT_K (WEAK_MT_PREFIX "k")
#define WEAK_MT_KV (WEAK_MT_PREFIX "kv")
#define WEAK_MT_V (WEAK_MT_PREFIX "v")

// -0, +3, pushes the three metatables for "k", "kv", and "v"
void ll_require_weak(lua_State *L);

static inline void ll_new_weak_table_k(lua_State *L, int narr, int nrec) {
  lua_createtable(L, narr, nrec);
  lua_getfield(L, LUA_REGISTRYINDEX, WEAK_MT_K);
  lua_setmetatable(L, -2);
}

static inline void ll_new_weak_table_kv(lua_State *L, int narr, int nrec) {
  lua_createtable(L, narr, nrec);
  lua_getfield(L, LUA_REGISTRYINDEX, WEAK_MT_KV);
  lua_setmetatable(L, -2);
}

static inline void ll_new_weak_table_v(lua_State *L, int narr, int nrec) {
  lua_createtable(L, narr, nrec);
  lua_getfield(L, LUA_REGISTRYINDEX, WEAK_MT_V);
  lua_setmetatable(L, -2);
}

#endif
