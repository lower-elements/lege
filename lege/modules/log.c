#include <SDL2/SDL_log.h>
#include <lauxlib.h>
#include <lua.h>

#include "preloads.h"
#include "util.h"

static const char *const LOG_PRIORITIES[SDL_NUM_LOG_PRIORITIES] = {
    "verbose", "debug", "info", "warn", "error", "critical", NULL};

static int log_get_priority(lua_State *L) {
  int idx = SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION) -
            SDL_LOG_PRIORITY_VERBOSE;
  lua_pushstring(L, LOG_PRIORITIES[idx]);
  return 1;
}

static int log_set_priority(lua_State *L) {
  SDL_LogPriority pri =
      luaL_checkoption(L, 1, NULL, LOG_PRIORITIES) + SDL_LOG_PRIORITY_VERBOSE;
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, pri);
  return 0;
}

static int log_message(lua_State *L) {
  SDL_LogPriority pri =
      luaL_checkoption(L, 1, NULL, LOG_PRIORITIES) + SDL_LOG_PRIORITY_VERBOSE;
  const char *str = luaL_checkstring(L, 2);
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, pri, "%s", str);
  return 0;
}

static int log_verbose(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static int log_debug(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static int log_info(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static int log_warn(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static int log_error(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static int log_critical(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

static const luaL_Reg LOG_FUNCS[] = {
    {.name = "get_priority", .func = log_get_priority},
    {.name = "set_priority", .func = log_set_priority},
    {.name = "message", .func = log_message},
    {.name = "verbose", .func = log_verbose},
    {.name = "debug", .func = log_debug},
    {.name = "info", .func = log_info},
    {.name = "warn", .func = log_warn},
    {.name = "error", .func = log_error},
    {.name = "critical", .func = log_critical},
    {NULL, NULL},
};

int luaopen_lege_log(lua_State *L) {
  lua_createtable(L, 0, arraysize(LOG_FUNCS) - 1);
  luaL_register(L, NULL, LOG_FUNCS);
  return 1;
}
