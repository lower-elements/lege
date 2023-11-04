#ifndef LIBLEGE_MOD_TASK_H
#define LIBLEGE_MOD_TASK_H

#include <lua.h>

void ll_require_task(lua_State *L);

extern const char TASK_EVERY_FRAME_KEY[];

void ll_task_run_frame(lua_State *L);

static inline void ll_task_get_every_frame_table(lua_State *L) {
  lua_getfield(L, LUA_REGISTRYINDEX, TASK_EVERY_FRAME_KEY);
}

#endif
