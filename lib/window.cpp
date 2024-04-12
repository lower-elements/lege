#include <SDL.h>
#include <lua.hpp>

#include "lege_internal.hpp"
#include "window.hpp"

typedef struct {
  SDL_Window *win;
} lege_window_t;

static int l_tostring(lua_State *L) {
  auto win = static_cast<lege_window_t *>(luaL_checkudata(L, 1, "lege.window"));
  const char *title = SDL_GetWindowTitle(win->win);
  lua_pushfstring(L, "window '%s': %p", title, lua_topointer(L, 1));
  return 1;
}

static int l_show(lua_State *L) {
  auto win = static_cast<lege_window_t *>(luaL_checkudata(L, 1, "lege.window"));
  SDL_ShowWindow(win->win);
  return 0;
}

static int l_hide(lua_State *L) {
  auto win = static_cast<lege_window_t *>(luaL_checkudata(L, 1, "lege.window"));
  SDL_HideWindow(win->win);
  return 0;
}

static int l_index(lua_State *L) {
  auto win = static_cast<lege_window_t *>(luaL_checkudata(L, 1, "lege.window"));
  const char *prop = luaL_checkstring(L, 2);
  if (SDL_strcmp(prop, "title") == 0) {
    lua_pushstring(L, SDL_GetWindowTitle(win->win));
  } else if (SDL_strcmp(prop, "id") == 0) {
    lua_pushnumber(L, (lua_Number)SDL_GetWindowID(win->win));
  } else if (SDL_strcmp(prop, "shown") == 0) {
    Uint32 flags = SDL_GetWindowFlags(win->win);
    lua_pushboolean(L, (flags & SDL_WINDOW_SHOWN) != 0);
  } else if (SDL_strcmp(prop, "show") == 0) {
    lua_pushcfunction(L, l_show);
  } else if (SDL_strcmp(prop, "hide") == 0) {
    lua_pushcfunction(L, l_hide);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static int l_newindex(lua_State *L) {
  auto win = static_cast<lege_window_t *>(luaL_checkudata(L, 1, "lege.window"));
  const char *prop = luaL_checkstring(L, 2);
  if (SDL_strcmp(prop, "title") == 0) {
    const char *title = luaL_checkstring(L, 3);
    SDL_SetWindowTitle(win->win, title);
  } else if (SDL_strcmp(prop, "shown") == 0) {
    luaL_checktype(L, 3, LUA_TBOOLEAN);
    int shown = lua_toboolean(L, 3);
    if (shown) {
      SDL_ShowWindow(win->win);
    } else {
      SDL_HideWindow(win->win);
    }
  } else {
    return luaL_error(L, "cannot set field '%s' on 'window' object", prop);
  }
  return 0;
}

void lege_create_global_window(lege_engine_t engine) {
  auto win = static_cast<lege_window_t *>(
      lua_newuserdata(engine->L, sizeof(lege_window_t)));
  win->win = engine->win;
  if (luaL_newmetatable(engine->L, "lege.window")) {
    lua_pushliteral(engine->L, "window");
    lua_setfield(engine->L, -2, "__name");
    lua_pushcfunction(engine->L, l_tostring);
    lua_setfield(engine->L, -2, "__tostring");
    lua_pushcfunction(engine->L, l_index);
    lua_setfield(engine->L, -2, "__index");
    lua_pushcfunction(engine->L, l_newindex);
    lua_setfield(engine->L, -2, "__newindex");
  }
  lua_setmetatable(engine->L, -2);
  lua_setfield(engine->L, LUA_GLOBALSINDEX, "window");
}
