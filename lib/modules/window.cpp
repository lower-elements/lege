#include <string_view>

#include <SDL.h>

#include "lua/helpers.hpp"

namespace lua = lege::lua;

namespace lege::modules {

static int l_tostring(lua_State *L) {
  auto win = *lua::check_userdata<SDL_Window *>(L, 1);
  const char *title = SDL_GetWindowTitle(win);
  lua_pushfstring(L, "window '%s': %p", title, lua_topointer(L, 1));
  return 1;
}

static int l_show(lua_State *L) {
  auto win = *lua::check_userdata<SDL_Window *>(L, 1);
  SDL_ShowWindow(win);
  return 0;
}

static int l_hide(lua_State *L) {
  auto win = *lua::check_userdata<SDL_Window *>(L, 1);
  SDL_HideWindow(win);
  return 0;
}

static int l_index(lua_State *L) {
  auto win = *lua::check_userdata<SDL_Window *>(L, 1);
  std::string_view prop;
  lua::arg(L, 2, prop);
  if (prop == "title") {
    lua_pushstring(L, SDL_GetWindowTitle(win));
  } else if (prop == "id") {
    lua_pushnumber(L, (lua_Number)SDL_GetWindowID(win));
  } else if (prop == "shown") {
    Uint32 flags = SDL_GetWindowFlags(win);
    lua_pushboolean(L, (flags & SDL_WINDOW_SHOWN) != 0);
  } else if (prop == "show") {
    lua_pushcfunction(L, l_show);
  } else if (prop == "hide") {
    lua_pushcfunction(L, l_hide);
  } else {
    // prop.data() *is* nul-terminated because it came from a Lua string
    return luaL_error(L, "cannot get field '%s' on 'window' object",
                      prop.data());
  }
  return 1;
}

static int l_newindex(lua_State *L) {
  auto win = *lua::check_userdata<SDL_Window *>(L, 1);
  std::string_view prop;
  lua::arg(L, 2, prop);
  if (prop == "title") {
    const char *title = luaL_checkstring(L, 3);
    SDL_SetWindowTitle(win, title);
  } else if (prop == "shown") {
    bool shown;
    if (lua::arg(L, 3, shown)) {
      SDL_ShowWindow(win);
    } else {
      SDL_HideWindow(win);
    }
  } else {
    // prop.data() *is* nul-terminated because it came from a Lua string
    return luaL_error(L, "cannot set field '%s' on 'window' object",
                      prop.data());
  }
  return 0;
}

void register_window_type(lua_State *L) {
  lua::make_metatable<SDL_Window *>(L);

  lua_pushliteral(L, "__name");
  lua_pushliteral(L, "Window");
  lua_rawset(L, -3);

  lua_pushliteral(L, "__metatable");
  lua_pushliteral(L, "window");
  lua_rawset(L, -3);

  lua_pushliteral(L, "__tostring");
  lua_pushcfunction(L, l_tostring);
  lua_rawset(L, -3);

  lua_pushliteral(L, "__index");
  lua_pushcfunction(L, l_index);
  lua_rawset(L, -3);

  lua_pushliteral(L, "__newindex");
  lua_pushcfunction(L, l_newindex);
  lua_rawset(L, -3);

  lua_pop(L, 1);
}

} // namespace lege::modules
