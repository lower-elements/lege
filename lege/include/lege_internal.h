#ifndef LIBLEGE_INTERNAL_H
#define LIBLEGE_INTERNAL_H

#include <lua.h>

typedef struct lege_engine {
  const char *app_name;
  const char *org_name;
  lua_State *L;
  int initialized_sdl_subsystems;
} *lege_engine_t;

// Get a pointer to the engine from an internal library
static inline lege_engine_t ll_get_engine(lua_State *L) {
  lua_getfield(L, LUA_REGISTRYINDEX, "lege.engine");
  lege_engine_t engine = lua_touserdata(L, -1);
  lua_pop(L, 1);
  return engine;
}

#endif
