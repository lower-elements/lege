#ifndef LIBLEGE_MOD_WEAK_H
#define LIBLEGE_MOD_WEAK_H

#include <lua.hpp>

namespace lege::weak {

#define WEAK_MT_PREFIX "lege.weak."
#define WEAK_MT_K (WEAK_MT_PREFIX "k")
#define WEAK_MT_KV (WEAK_MT_PREFIX "kv")
#define WEAK_MT_V (WEAK_MT_PREFIX "v")

// -0, +3, pushes the three metatables for "k", "kv", and "v"
void require(lua_State *L);

static inline void new_k(lua_State *L, int narr = 0, int nrec = 0) {
  lua_createtable(L, narr, nrec);
  luaL_setmetatable(L, WEAK_MT_K);
}

static inline void new_kv(lua_State *L, int narr = 0, int nrec = 0) {
  lua_createtable(L, narr, nrec);
  luaL_setmetatable(L, WEAK_MT_KV);
}

static inline void new_v(lua_State *L, int narr = 0, int nrec = 0) {
  lua_createtable(L, narr, nrec);
  luaL_setmetatable(L, WEAK_MT_V);
}

} // namespace lege::weak

#endif
