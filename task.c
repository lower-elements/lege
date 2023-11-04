#include <lauxlib.h>
#include <lua.h>

#include "modules/task.h"
#include "preloads.h"

const char TASK_EVERY_FRAME_KEY[] = "lege.task.every_frame";

static int l_every_frame(lua_State *L) {
  luaL_checktype(L, 1, LUA_TFUNCTION);
  lua_settop(L, 1);
  lua_getfield(L, LUA_REGISTRYINDEX, TASK_EVERY_FRAME_KEY);
  lua_insert(L, 1); // 1 = table, 2 = function
  lua_Integer num_tasks = (lua_Integer)lua_objlen(L, 1);
  lua_rawseti(L, 1, num_tasks + 1);
  return 0;
}

void ll_task_run_frame(lua_State *L) {
  ll_task_get_every_frame_table(L);
  lua_Integer num_tasks = (lua_Integer)lua_objlen(L, -1);
  for (lua_Integer i = 1; i <= num_tasks; ++i) {
    lua_rawgeti(L, -1, i);
    lua_call(L, 0, 0);
  }
  lua_pop(L, 1);
}

static const luaL_Reg TASK_FUNCS[] = {
    {.name = "every_frame", .func = l_every_frame},
    {.name = NULL, .func = NULL}};

void ll_require_task(lua_State *L) {
  // Create the table that stores tasks to be run every frame
  // This is not a metatable but this function conveniently never recreates the
  // table
  (void)luaL_newmetatable(L, TASK_EVERY_FRAME_KEY);
  // We start with 0 tasks
  lua_pushliteral(L, "n");
  lua_pushnumber(L, 0);
  lua_rawset(L, -3);
}

int luaopen_lege_task(lua_State *L) {
  ll_require_task(L);
  // Create and return the library table of functions
  luaL_newlib(L, TASK_FUNCS);
  return 1;
}
