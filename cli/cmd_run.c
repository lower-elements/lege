#include <lauxlib.h>
#include <lege.h>
#include <lua.h>
#include <lualib.h>
#include <optparse.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"

bool get_and_check_type(lua_State *L, int tbl, const char *key, int type) {
  lua_pushstring(L, key);
  lua_gettable(L, tbl);
  if (lua_type(L, -1) != type) {
    fprintf(stderr, "%s: Error: Expected \"%s\" to be a %s, got %s\n",
            PROG_NAME, key, lua_typename(L, type), luaL_typename(L, -1));
    return false;
  }
  return true;
}

void cmd_run(struct optparse *opts) {
  (void)opts;

  // Create Lua state
  lua_State *L = luaL_newstate();
  if (!L) {
    fprintf(stderr, "%s: Error: Failed to create Lua state\n", PROG_NAME);
    exit(EXIT_FAILURE);
  }
  luaL_openlibs(L);

  // Load the project manifest
  int res = luaL_dofile(L, "project.lua");
  // Todo: Better error handling here
  if (res != LUA_OK) {
    fprintf(stderr, "%s: Error: Failed to load \"project.lua\"\n", PROG_NAME);
    goto close_lua;
  }

  // Get the project table
  if (!get_and_check_type(L, LUA_GLOBALSINDEX, "project", LUA_TTABLE)) {
    goto close_lua;
  }
  // Get project metadata
  if (!get_and_check_type(L, 1, "name", LUA_TSTRING)) {
    goto close_lua;
  }
  const char *name = lua_tostring(L, -1);
  if (!get_and_check_type(L, 1, "organization", LUA_TSTRING)) {
    goto close_lua;
  }
  const char *org = lua_tostring(L, -1);
  if (!get_and_check_type(L, 1, "entrypoint", LUA_TSTRING)) {
    goto close_lua;
  }
  const char *entrypoint = lua_tostring(L, -1);

  // Create the engine
  lege_engine_t engine = lege_engine_new();
  if (!engine) {
    fprintf(stderr, "%s: Error: Failed to create LEGE engine\n", PROG_NAME);
    goto close_lua;
  }

  // Set engine properties
  if (!lege_engine_set_string(engine, LEGE_OPTION_APP_NAME, name)) {
    fprintf(stderr, "%s: Error: Could not set application name\n", PROG_NAME);
    goto free_engine;
  }
  if (!lege_engine_set_string(engine, LEGE_OPTION_ORG_NAME, org)) {
    fprintf(stderr, "%s: Error: Could not set organization name\n", PROG_NAME);
    goto free_engine;
  }

  // Preload other scripts (if any)
  lua_getglobal(L, "preload");
  if (lua_isnil(L, 5)) {
    goto load_entrypoint;
  } else if (lua_istable(L, 5)) {
    // Preload each file
    for (lua_pushnil(L); lua_next(L, 5) != 0; lua_pop(L, 1)) {
      const char *fname = lua_tostring(L, -1);
      lege_engine_preload_file(engine, fname, fname);
    }
  } else {
    fprintf(stderr,
            "%s: Error: Expected \"preload\" to be a table or nil, got %s\n",
            PROG_NAME, luaL_typename(L, -1));
    goto free_engine;
  }

  // Load the entrypoint script
load_entrypoint:
  if (!lege_engine_load_file(engine, entrypoint)) {
    fprintf(stderr, "%s: Error: Failed to load entrypoint from \"%s\"\n",
            PROG_NAME, entrypoint);
    goto free_engine;
  }

  lege_engine_run(engine);

  lege_engine_free(engine);
  lua_close(L);
  return;

  // Cleanup on error
free_engine:
  lege_engine_free(engine);
close_lua:
  lua_close(L);
  exit(EXIT_FAILURE);
}
