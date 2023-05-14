#ifndef LIBLEGE_PRELOADS_H
#define LIBLEGE_PRELOADS_H

#include <lua.h>

void lege_preload_builtins(lua_State *L);

// Prototypes for builtin module loader functions
int load_log_module(lua_State *L);

#endif
