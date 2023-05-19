#include <SDL2/SDL_log.h>
#include <lauxlib.h>
#include <lua.h>

#include "preloads.h"
#include "util.h"

/**
 * Logging facilities for LEGE programs. This module uses [SDL's logging
 * facilities][sdl-log] to write log messages to the appropriate place for each
 * platform.
 *
 * [sdl-log]: <https://wiki.libsdl.org/SDL3/CategoryLog>
 * @usage
 * local log = require "lege.log"
 *
 * log.info("Log priority is currently set to " .. log.get_priority())
 * log.verbose("So you won't see this")
 * log.set_priority("verbose")
 * log.info("Log priority is now set to " .. log.get_priority())
 *
 * log.verbose("The game's still running")
 * log.debug("Starting frobnication process ...")
 * log.info("Logged in as xx_DankMemer69_xx")
 * log.warn("That username is stupid")
 * log.error("The server rejected your username, as it is too stupid")
 * log.critical("Cannot continue with a user this stupid")
 *
 * local msg_type = "info"
 * log.message(msg_type, "Yes, we're talking about *you*")
 * @module lege.log
 */

static const char *const LOG_PRIORITIES[SDL_NUM_LOG_PRIORITIES] = {
    "verbose", "debug", "info", "warn", "error", "critical", NULL};

/**
 * Get the lowest log priority at which log messages are output.
 * @function get_priority
 * @treturn string The current SDL log priority
 * @see set_priority
 * @see sdl:SDL_LogGetPriority
 */
static int log_get_priority(lua_State *L) {
  int idx = SDL_LogGetPriority(SDL_LOG_CATEGORY_APPLICATION) -
            SDL_LOG_PRIORITY_VERBOSE;
  lua_pushstring(L, LOG_PRIORITIES[idx]);
  return 1;
}

/**
 * Set the lowest log priority at which log messages are output.
 * @function set_priority
 * @tparam string priority The new SDL log priority
 * @raise if priority is not a valid log message priority
 * @see get_priority
 * @see sdl:SDL_LogSetPriority
 */
static int log_set_priority(lua_State *L) {
  SDL_LogPriority pri =
      luaL_checkoption(L, 1, NULL, LOG_PRIORITIES) + SDL_LOG_PRIORITY_VERBOSE;
  SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, pri);
  return 0;
}

/**
 * Log a message at the specified priority.
 * @function message
 * @tparam string priority The priority of the log message
 * @tparam string message The message to log
 * @raise if priority is not a valid log message priority
 * @see sdl:SDL_LogMessage
 */
static int log_message(lua_State *L) {
  SDL_LogPriority pri =
      luaL_checkoption(L, 1, NULL, LOG_PRIORITIES) + SDL_LOG_PRIORITY_VERBOSE;
  const char *str = luaL_checkstring(L, 2);
  SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, pri, "%s", str);
  return 0;
}

/**
 * Log a message at the verbose priority.
 * @function verbose
 * @tparam string message The message to log
 * @see sdl:SDL_LogVerbose
 */
static int log_verbose(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

/**
 * Log a message at the debug priority.
 * @function debug
 * @tparam string message The message to log
 * @see sdl:SDL_LogDebug
 */
static int log_debug(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

/**
 * Log a message at the info priority.
 * @function info
 * @tparam string message The message to log
 * @see sdl:SDL_LogInfo
 */
static int log_info(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

/**
 * Log a message at the warn priority.
 * @function warn
 * @tparam string message The message to log
 * @see sdl:SDL_LogWarn
 */
static int log_warn(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

/**
 * Log a message at the error priority.
 * @function error
 * @tparam string message The message to log
 * @see sdl:SDL_LogError
 */
static int log_error(lua_State *L) {
  const char *str = luaL_checkstring(L, 1);
  SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", str);
  return 0;
}

/**
 * Log a message at the critical priority.
 * @function critical
 * @tparam string message The message to log
 * @see sdl:SDL_LogCritical
 */
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
  luaL_newlib(L, LOG_FUNCS);
  return 1;
}
