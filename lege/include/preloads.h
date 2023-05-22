#ifndef LIBLEGE_PRELOADS_H
#define LIBLEGE_PRELOADS_H

#include <lua.h>

void lege_preload_builtins(lua_State *L);

// Prototypes for builtin module loader functions
int luaopen_lege_enum(lua_State *L);
int luaopen_lege_log(lua_State *L);
int luaopen_lege_readonly(lua_State *L);
int luaopen_lege_strict(lua_State *L);
int luaopen_lege_struct(lua_State *L);

#endif
