#include <lauxlib.h>
#include <lege.h>
#include <lua.h>
#include <lualib.h>
#include <optparse.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "error_reporting.h"
#include "util.h"

bool get_and_check_type(lua_State *L, int tbl, const char *key, int type) {
  lua_pushstring(L, key);
  lua_gettable(L, tbl);
  if (lua_type(L, -1) != type) {
    error("Expected \"%s\" to be a %s, got %s", key, lua_typename(L, type),
          luaL_typename(L, -1));
    return false;
  }
  return true;
}

void cmd_run(struct optparse *opts) {
  (void)opts;

  // Create Lua state
  lua_State *L = luaL_newstate();
  if (!L) {
    fatal(" Failed to create Lua state");
  }
  luaL_openlibs(L);

  // Load the project manifest
  int res = luaL_dofile(L, "project.lua");
  // Todo: Better error handling here
  if (res != LUA_OK) {
    fatal_cleanup(close_lua, "Failed to load \"project.lua\"");
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
    fatal_cleanup(close_lua, "Failed to create LEGE engine");
  }

  // Set engine properties
  if (!lege_engine_set_string(engine, LEGE_OPTION_APP_NAME, name)) {
    fatal_cleanup(free_engine, "Could not set application name");
  }
  if (!lege_engine_set_string(engine, LEGE_OPTION_ORG_NAME, org)) {
    fatal_cleanup(free_engine, "Could not set organization name");
  }

  // Preload other scripts (if any)
  lua_getglobal(L, "preload");
  switch (lua_type(L, 5)) {
  case LUA_TNONE:
  case LUA_TNIL:
    goto load_entrypoint;
  case LUA_TTABLE:
    // Preload each file
    ll_for_each_pair(L, 5) {
      const char *fname = lua_tostring(L, -1);
      // Get the package name
      const char *pkg_name = fname;
      if (lua_type(L, -2) == LUA_TSTRING) {
        pkg_name = lua_tostring(L, -2);
      } else if (!lua_isnumber(L, -2)) {
        warn("Found a key of type %s in \"preload\", skipping it",
             luaL_typename(L, -2));
        continue;
      }
      // Read the Lua file into a buffer
      size_t size;
      const char *buf = lege_read_to_buf(fname, &size);
      if (!buf) {
        fatal_cleanup(free_engine, "Failed to preload \"%s\"", fname);
      }
      // Tell the engine to preload it
      lege_engine_preload(engine, buf, size, pkg_name);
    }
    break;
  default:
    fatal_cleanup(free_engine,
                  "Expected \"preload\" to be a table or nil, got %s",
                  luaL_typename(L, -1));
  }

  // Load the entrypoint script
load_entrypoint:
  if (!lege_engine_load_file(engine, entrypoint)) {
    fatal_cleanup(free_engine, "Failed to load entrypoint from \"%s\"",
                  entrypoint);
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
