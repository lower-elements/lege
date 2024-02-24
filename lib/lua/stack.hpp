#ifndef LIBLEGE_LUA_STAK_HPP
#define LIBLEGE_LUA_STAK_HPP

#include <optional>
#include <string>
#include <string_view>

#include <lua.hpp>

#include "lua/nil.hpp"

namespace lege::lua {

class TableView;

static inline int absindex(lua_State *L, int index) {
  return index >= 0 ? index : lua_gettop(L) + index + 1;
}

void get(lua_State *L, int index, lua_Number &val);
void get(lua_State *L, int index, bool &val);
void get(lua_State *L, int index, std::string_view &val);
void get(lua_State *L, int index, std::string &val);
void get(lua_State *L, int index, const char *&val);
void get(lua_State *L, int index, lua_CFunction &val);
void get(lua_State *L, int index, void *&val);
void get(lua_State *L, int index, lua_Integer &val);
void get(lua_State *L, int index, lua_State *&val);
void get(lua_State *L, int index, TableView &val);

void arg(lua_State *L, int index, lua_Number &val);
void arg(lua_State *L, int index, bool &val);
void arg(lua_State *L, int index, std::string_view &val);
void arg(lua_State *L, int index, std::string &val);
void arg(lua_State *L, int index, const char *&val);
void arg(lua_State *L, int index, lua_CFunction &val);
void arg(lua_State *L, int index, void *&val, const char *mt);
void arg(lua_State *L, int index, lua_Integer &val);
void arg(lua_State *L, int index, lua_State *&val);
void arg(lua_State *L, int index, TableView &val);

void opt_arg(lua_State *L, int index, lua_Number &val, lua_Number def = 0.0);
void opt_arg(lua_State *L, int index, bool &val, bool def = 0);
void opt_arg(lua_State *L, int index, std::string_view &val,
             std::string_view def = "");
void opt_arg(lua_State *L, int index, std::string &val, std::string def = "");
void opt_arg(lua_State *L, int index, const char *&val, const char *def = "");
void opt_arg(lua_State *L, int index, lua_CFunction &val,
             lua_CFunction def = nullptr);
void opt_arg(lua_State *L, int index, void *&val, const char *mt,
             void *def = nullptr);
void opt_arg(lua_State *L, int index, lua_Integer &val, lua_Integer def = 0);
void opt_arg(lua_State *L, int index, lua_State *&val,
             lua_State *def = nullptr);
void opt_arg(lua_State *L, int index, std::optional<TableView> &val);

void push(lua_State *L, Nil);
void push(lua_State *L, lua_Number val);
void push(lua_State *L, bool val);
void push(lua_State *L, std::string_view val);
void push(lua_State *L, const std::string &val);
void push(lua_State *L, const char *val);
void push(lua_State *L, lua_CFunction val, int nups = 0);
void push(lua_State *L, void *val);
void push(lua_State *L, lua_Integer val);

} // namespace lege::lua

#endif
