#include <SDL.h>
#include <hedley.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdbool.h>
#include <string.h>

#include "lege.h"
#include "lege_internal.h"
#include "preloads.h"
#include "util.h"

static int on_panic(lua_State *L) {
  const char *err = lua_tostring(L, -1);
  luaL_traceback(L, L, err, 0);
  const char *tb = lua_tostring(L, -1);
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Runtime error: %s", tb);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Runtime Error", tb, NULL);
  return 0;
}

lege_engine_t lege_engine_new(void) {
  // Allocate the engine object
  lege_engine_t engine = lege_xnew(struct lege_engine);
  if (HEDLEY_UNLIKELY(!engine)) {
    return NULL;
  }

  // Set default options
  engine->app_name = "LEGE Application";
  engine->org_name = "lower-elements";

  // Initialize the Lua state
  engine->L = luaL_newstate();
  if (HEDLEY_UNLIKELY(!engine->L)) {
    goto err;
  }
  lua_atpanic(engine->L, on_panic);
  luaL_openlibs(engine->L);
  // Store a pointer to the engine in the registry
  lua_pushlightuserdata(engine->L, engine);
  lua_setfield(engine->L, LUA_REGISTRYINDEX, "lege.engine");
  lege_preload_builtins(engine->L);
  lua_settop(engine->L, 0);
  lua_pushcfunction(engine->L, on_panic);

  // Set SDL hints
  SDL_SetHintWithPriority(SDL_HINT_IME_INTERNAL_EDITING, "1", SDL_HINT_DEFAULT);
  SDL_SetHintWithPriority(SDL_HINT_IME_SHOW_UI, "1", SDL_HINT_DEFAULT);
  SDL_SetHintWithPriority(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT, "1",
                          SDL_HINT_DEFAULT);

  return engine;

err:
  lege_free(engine);
  return NULL;
}

void lege_engine_free(lege_engine_t engine) {
  if (HEDLEY_LIKELY(engine)) {
    SDL_QuitSubSystem(engine->initialized_sdl_subsystems);
    lua_close(engine->L);
    lege_free(engine);
  }
}

bool lege_engine_set_string(lege_engine_t engine, lege_option_t opt,
                            const char *str) {
  switch (opt) {
  case LEGE_OPTION_APP_NAME:
    engine->app_name = str;
    if (!SDL_SetHint(SDL_HINT_APP_NAME, str)) {
      SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                  "Could not set application name to \"%s\"", str);
    }
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
  return luaL_loadbuffer(engine->L, buff, sz, name) == LUA_OK;
}

bool lege_engine_load_file(lege_engine_t engine, const char *fname) {
  return luaL_loadfile(engine->L, fname) == LUA_OK;
}

static int load_preloaded(lua_State *L) {
  // Check first argument (the package to load) is a string
  const char *pkg = luaL_checkstring(L, 1);
  // Check it's the same as the name of the package this closure loads
  lua_pushvalue(L, lua_upvalueindex(1));
  if (!lua_rawequal(L, 1, -1)) {
    // Not the same package
    return luaL_error(L, "Failed to load package %s", pkg);
  }

  // Retrieve and call the function for this particular package
  lua_pushvalue(L, lua_upvalueindex(2));
  lua_call(L, 0, 1);
  return 1;
}

void lege_engine_preload(lege_engine_t engine, const char *buff, size_t sz,
                         const char *name) {
  // Get the package.preload table
  lua_getglobal(engine->L, "package");
  lua_pushliteral(engine->L, "preload");
  lua_rawget(engine->L, -2);
  SDL_assert(lua_type(engine->L, -1) == LUA_TTABLE);
  // Push the key we want to set on package.preload
  lua_pushstring(engine->L, name);
  // Push it again, as it's the first upvalue for our preload closure
  lua_pushstring(engine->L, name);
  // Load code from the buffer, storing it as a function at the stack top
  luaL_loadbuffer(engine->L, buff, sz, name);
  // Construct a closure with the two top stack items
  // This closure runs the preloaded code, hense loading the package
  lua_pushcclosure(engine->L, load_preloaded, 2);
  // package.preload[name] = closure
  lua_rawset(engine->L, -3);
}

void lege_engine_preload_file(lege_engine_t engine, const char *fname,
                              const char *name) {
  (void)name;
  // Get the package.preload table
  lua_getglobal(engine->L, "package");
  lua_pushliteral(engine->L, "preload");
  lua_rawget(engine->L, -2);
  // Push the key we want to set on package.preload
  lua_pushstring(engine->L, fname);
  // Push it again, as it's the first upvalue for our preload closure
  lua_pushstring(engine->L, fname);
  // Load code from the buffer, storing it as a function at the stack top
  luaL_loadfile(engine->L, fname);
  // Construct a closure with the two top stack items
  // This closure runs the preloaded code, hense loading the package
  lua_pushcclosure(engine->L, load_preloaded, 2);
  // package.preload[name] = closure
  lua_rawset(engine->L, -3);
}

void lege_engine_run(lege_engine_t engine) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Starting %s.%s\n",
              engine->org_name, engine->app_name);
  // We left the main chunk on the stack top
  // Todo: Make this more robust by storing it in the registry
  lua_pcall(engine->L, 0, 0, 1);
}
