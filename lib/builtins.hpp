#ifndef LIBLEGE_BUILTINS_HPP
#define LIBLEGE_BUILTINS_HPP

#include "engine.hpp"

extern "C" {
int luaopen_lege_enum(lua_State *L);
int luaopen_lege_log(lua_State *L);
int luaopen_lege_readonly(lua_State *L);
int luaopen_lege_strict(lua_State *L);
int luaopen_lege_struct(lua_State *L);
int luaopen_lege_weak(lua_State *L);
int luaopen_utf8(lua_State *L);
}

namespace lege::modules {

void register_builtins(EngineImpl &e);

}

#endif
