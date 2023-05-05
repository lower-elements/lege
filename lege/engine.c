#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <stdio.h>

#include "lege.h"
#include "util.h"

struct lege_engine {
  const char *app_name;
  const char *org_name;
  lua_State *L;
};

lege_engine_t lege_engine_new(void) {
  // Allocate the engine object
  lege_engine_t engine = lege_xnew(struct lege_engine);
  if (UNLIKELY(!engine)) {
    return NULL;
  }

  // Set default options
  engine->app_name = "LEGE Application";
  engine->org_name = "lower-elements";

  // Initialize the Lua state
  engine->L = luaL_newstate();
  if (UNLIKELY(!engine->L)) {
    goto err;
  }
  luaL_openlibs(engine->L);

  return engine;

err:
  lege_free(engine);
  return NULL;
}

void lege_engine_free(lege_engine_t engine) {
  if (LIKELY(engine)) {
    lua_close(engine->L);
    lege_free(engine);
  }
}

bool lege_engine_set_string(lege_engine_t engine, lege_option_t opt,
                            const char *str) {
  switch (opt) {
  case LEGE_OPTION_APP_NAME:
    engine->app_name = str;
    return true;
  case LEGE_OPTION_ORG_NAME:
    engine->org_name = str;
    return true;
  default:
    return false; // Option not recognised
  }
}

bool lege_engine_load(lege_engine_t engine, const char *buff, size_t sz,
                      const char *name) {
  return luaL_loadbuffer(engine->L, buff, sz, name) != 0;
}

void lege_engine_run(lege_engine_t engine) {
  printf("Starting %s.%s\n", engine->org_name, engine->app_name);
  lua_call(engine->L, 0, 0);
}
