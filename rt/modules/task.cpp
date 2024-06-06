#include "lua/helpers.hpp"
#include "modules/weak.hpp"

namespace lua = lege::lua;
namespace weak = lege::weak;

#define ENV_NAME "lege.task.env"
#define MT_NAME "lege.task.mt"
// this is useful for more than just metatables
#define luaL_newregistrytable(L, name) (luaL_newmetatable((L), (name)))

static int l_get_support_tables(lua_State *L) {
  lua_pushvalue(L, lua_upvalueindex(1));
  return 1;
}

static bool get_current_task(lua_State *L, bool pop_global_env = true) {
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushliteral(L, "by_thread");
  lua_rawget(L, -2);
  if (pop_global_env) {
    lua_replace(L, -2);
  }
  bool is_main = lua_pushthread(L);
  lua_rawget(L, -2);
  lua_replace(L, -2);
  return is_main;
}

static bool get_current_task_env(lua_State *L) {
  bool is_main = get_current_task(L);
  lua_getfenv(L, -1);
  lua_replace(L, -2);
  return is_main;
}

static bool get_current_task_children(lua_State *L) {
  bool is_main = get_current_task_env(L);
  if (is_main) {
    lua_pushvalue(L, lua_upvalueindex(1));
    lua_replace(L, -2);
    lua_pushliteral(L, "toplevels");
  } else {
    lua_pushliteral(L, "children");
  }
  lua_rawget(L, -2);
  lua_replace(L, -2);
  return is_main;
}

static int l_current(lua_State *L) {
  get_current_task(L);
  return 1;
}

static int l_tostring(lua_State *L) {
  luaL_checkudata(L, 1, MT_NAME);
  lua_settop(L, 1);

  lua_getfenv(L, 1);
  lua_pushliteral(L, "name");
  lua_rawget(L, -2);
  const char *name = lua_tostring(L, -1);

  lua_pushfstring(L, "Task '%s': %p", name, lua_topointer(L, 1));
  return 1;
}

static int l_index(lua_State *L) {
  luaL_checkudata(L, 1, MT_NAME);
  lua_settop(L, 2);

  // Fields come from the environment table
  lua_getfenv(L, 1);
  lua_pushvalue(L, 2);
  lua_rawget(L, -2);

  if (lua_isnil(L, -1)) {
    // Raise a helpful error message instead of just returning nil
    const char *field = lua_tostring(L, 2);
    luaL_callmeta(L, 1, "__tostring");
    const char *task = lua_tostring(L, -1);
    return luaL_error(L, "No field '%s' on %s", field, task);
  }

  return 1;
}

static int l_newindex(lua_State *L) {
  luaL_checkudata(L, 1, MT_NAME);

  const char *field = lua_tostring(L, 2);
  luaL_callmeta(L, 1, "__tostring");
  const char *task = lua_tostring(L, -1);
  return luaL_error(L, "Cannot set field '%s' on %s", field, task);
}

static void make_task(lua_State *L, int nameindex, int funcindex) {
  nameindex = lua::absindex(L, nameindex);
  funcindex = lua::absindex(L, funcindex);

  // Make the new task
  lua_newuserdata(L, 0);

  // Make the coroutine the task runs on
  lua_State *co = lua_newthread(L);
  // Push the task's function on the coroutine's stack
  lua_pushvalue(L, funcindex);
  lua_xmove(L, co, 1);

  if (luaL_newmetatable(L, MT_NAME)) {
    // Set up metatable
    lua_pushliteral(L, "__tostring");
    lua_pushcfunction(L, l_tostring);
    lua_rawset(L, -3);

    lua_pushliteral(L, "__index");
    lua_pushcfunction(L, l_index);
    lua_rawset(L, -3);

    lua_pushliteral(L, "__newindex");
    lua_pushcfunction(L, l_newindex);
    lua_rawset(L, -3);
  }
  lua_setmetatable(L, -3);

  // Create an associated environment table, used to store Lua values
  lua_createtable(L, 0, 2);

  // env.name = name
  lua_pushliteral(L, "name");
  lua_pushvalue(L, nameindex);
  lua_rawset(L, -3);

  // env.children = {}
  lua_pushliteral(L, "children");
  lua_newtable(L);
  lua_rawset(L, -3);

  // Env.co = co
  lua_pushliteral(L, "co");
  lua_pushvalue(L, -3);
  lua_rawset(L, -3);

  // env.parent = parent
  lua_pushliteral(L, "parent");
  bool parent_is_main = get_current_task(L);
  if (parent_is_main) {
    // We still want a non-nil value so that __index doesn't error
    lua_pushboolean(L, false);
    lua_replace(L, -2);
  }
  lua_rawset(L, -3);

  lua_setfenv(L, -3);

  // global_env.by_thread[co] = task
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushliteral(L, "by_thread");
  lua_rawget(L, -2);
  lua_pushvalue(L, -3); // Coroutine
  lua_pushvalue(L, -5); // Task
  lua_rawset(L, -3);

  lua_pop(L, 1); // Pop by_thread

  // Schedule the task
  lua_pushliteral(L, "pending");
  lua_rawget(L, -2);
  lua_pushvalue(L, -4); // Task
  lua_pushboolean(L, true);
  lua_rawset(L, -3);

  lua_pop(L, 3); // Pop pending, global_env, and co
}

static int l_spawn(lua_State *L) {
  luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);
  lua_settop(L, 2);

  make_task(L, 1, 2);

  get_current_task_children(L);
  lua_pushvalue(L, 3);
  lua_pushboolean(L, true);
  lua_rawset(L, -3);

  lua_settop(L, 3);
  return 1;
}

static int l_block(lua_State *L) {
  if (!lua_isyieldable(L)) {
    return luaL_error(L, "Cannot block the main task");
  }

  lua_settop(L, 0);

  // false == don't pop the global environment
  get_current_task(L, false);

  // global_env.pending[current_task] = nil
  lua_pushliteral(L, "pending");
  lua_rawget(L, -3);
  lua_pushvalue(L, -2); // Current task
  lua_pushnil(L);
  lua_rawset(L, -3);

  lua_pushliteral(L, "blocked");
  lua_rawget(L, -4);    // global_env
  lua_pushvalue(L, -3); // Current task
  lua_pushboolean(L, true);
  lua_rawset(L, -3);

  return lua_yield(L, 0);
}

static const luaL_Reg TASK_FUNCS[]{{"get_support_tables", l_get_support_tables},
                                   {"current", l_current},
                                   {"spawn", l_spawn},
                                   {"block", l_block},
                                   {nullptr, nullptr}};

static void make_support_tables(lua_State *L) {
  if (luaL_newregistrytable(L, ENV_NAME)) {
    lua_pushliteral(L, "by_thread");
    weak::new_kv(L);
    lua_rawset(L, -3);

    // Tasks that can run
    lua_pushliteral(L, "pending");
    lua_newtable(L);
    lua_rawset(L, -3);

    // Tasks waiting for something (probably I/O)
    lua_pushliteral(L, "blocked");
    lua_newtable(L);
    lua_rawset(L, -3);

    // Tasks spawned from the main thread
    lua_pushliteral(L, "toplevels");
    lua_newtable(L);
    lua_rawset(L, -3);
  }
}

extern "C" int luaopen_lege_task(lua_State *L) {
  weak::require(L);
  luaL_newlibtable(L, TASK_FUNCS);
  make_support_tables(L);
  luaL_setfuncs(L, TASK_FUNCS, 1);
  return 1;
}
