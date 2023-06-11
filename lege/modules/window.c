#include <SDL.h>
#include <lauxlib.h>
#include <limits.h>
#include <lua.h>

#include "lege_internal.h"
#include "modules/weak.h"
#include "modules/window.h"

/**
 * Window creation and management.
 * @module lege.window
 * @return A function, @{Window}, for creating new Windows
 */

static const char *const WINDOW_MT_NAME = "lege.window.Window";

static void ll_tbl_optint(lua_State *L, int tbl, const char *key, int *out) {
  lua_getfield(L, tbl, key);
  int is_number;
  lua_Number num = lua_tointegerx(L, -1, &is_number);
  if (!is_number && !lua_isnil(L, -1)) {
    luaL_error(L, "Expected Window %s to be  number or nil, got %s", key,
               luaL_typename(L, -1));
  }
  lua_pop(L, 1);
  SDL_assert(num >= (lua_Integer)INT_MIN);
  SDL_assert(num <= (lua_Integer)INT_MAX);
  *out = (int)num;
}

/**
 * Create a new Window.
 * @function Window
 * @tparam table options Options for the new window. Available options are:
 * `title`, `width` and `height`.
 * @return A Window object
 */
static int l_window(lua_State *L) {
  lua_settop(L, 1); // Discard any values except the first

  // Default values for Window options
  const char *title = "LEGE Application";
  int w = 640, h = 480;
  Uint32 flags = 0;

  if (lua_istable(L, 1)) {
    // Width and height
    ll_tbl_optint(L, 1, "width", &w);
    ll_tbl_optint(L, 1, "height", &h);
    // Title. We must leave the string on the stack until after we call
    // SDL_CreateWindow
    lua_getfield(L, 1, "title");
    const char *str = lua_tostring(L, 2);
    if (str) {
      title = str;
    } else if (!lua_isnil(L, 2)) {
      luaL_error(L, "Expected Window title to be string or nil, got %s",
                 luaL_typename(L, 2));
    }
  }

  // Create the window Lua object
  lege_window_t *win = lua_newuserdata(L, sizeof(lege_window_t));
  win->win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, w, h, flags);
  if (!win->win) {
    return luaL_error(L, "Could not create window: %s", SDL_GetError());
  }
  win->ren = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_PRESENTVSYNC);
  if (!win->ren) {
    return luaL_error(L, "Could not create renderer: %s", SDL_GetError());
  }

  // Set metatable
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_setmetatable(L, -2);

  // Add the Window object to the weak table of windows by id
  lua_pushvalue(L, lua_upvalueindex(2));
  Uint32 window_id = SDL_GetWindowID(win->win);
  lua_pushnumber(L, (lua_Number)window_id);
  lua_pushvalue(L, -3);
  lua_rawset(L, -3);
  lua_pop(L, 1);

  return 1;
}

/**
 * Convert a @{Window} to a human-readable string.
 * Currently we merely output "Window" and its address.
 * @meta Window.__tostring
 * @tparam Window self The Window to convert to a string
 * @treturn string A formatted string "Window: &lt;address&gt;"
 */
static int l_window_tostring(lua_State *L) {
  const void *ptr = lua_topointer(L, 1);
  lua_pushfstring(L, "Window: %p", ptr);
  return 1;
}

static int l_window_gc(lua_State *L) {
  lege_window_t *win = luaL_checkudata(L, 1, WINDOW_MT_NAME);
  SDL_DestroyRenderer(win->ren);
  win->ren = NULL;
  SDL_DestroyWindow(win->win);
  win->win = NULL;
  return 0;
}

int luaopen_lege_window(lua_State *L) {
  if (SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    return luaL_error(L, "Failed to initialize video subsystem: %s",
                      SDL_GetError());
  }
  lege_engine_t engine = ll_get_engine(L);
  engine->initialized_sdl_subsystems |= SDL_INIT_VIDEO;
  if (luaL_newmetatable(L, WINDOW_MT_NAME)) {
    lua_pushliteral(L, "Window");
    lua_setfield(L, 1, "__name");
    lua_pushcfunction(L, l_window_gc);
    lua_setfield(L, 1, "__gc");
    lua_pushcfunction(L, l_window_tostring);
    lua_setfield(L, 1, "__tostring");
  }

  // We need weak table metatables
  ll_require_weak(L);
  lua_settop(L, 1);

  // Create the weak table of windows, used for directing SDL events to the
  // correct handlers, among other things
  if (luaL_newmetatable(L, WINDOWS_TBL_NAME)) {
    // Set this table as weak 'v'
    lua_getfield(L, LUA_REGISTRYINDEX, WEAK_MT_V);
    lua_setmetatable(L, 2);
  }

  lua_pushcclosure(L, l_window, 2);
  return 1;
}
