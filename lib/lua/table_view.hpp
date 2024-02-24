#ifndef LIBLEGE_LUA_TABLE_VIEW_HPP
#define LIBLEGE_LUA_TABLE_VIEW_HPP

#include <optional>
#include <string>
#include <string_view>

#include <lua.hpp>

#include "lua/nil.hpp"

namespace lege::lua {

template <class K> class TableValProxy;

class TableView {
public:
  TableView(lua_State *state, int index);

  static TableView globals(lua_State *L);
  static TableView registry(lua_State *L);

  template <class K> TableValProxy<K> operator[](K key) {
    return TableValProxy(L, m_index, key);
  }

private:
  lua_State *L;
  int m_index;
};

template <class K> class TableValProxy {
private:
  TableValProxy(lua_State *state, int index, K k)
      : L(state), m_index(index), key(k) {}

public:
  void pushKey() = delete;
  void get() {
    int index = m_index >= 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_gettable(L, index);
  }

  TableValProxy<K> &set(Nil) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushnil(L);
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(lua_Number val) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushnumber(L, val);
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(bool val) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushboolean(L, val);
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(std::string_view val) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushlstring(L, val.data(), val.size());
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(const char *val) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushstring(L, val);
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(void *val) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushlightuserdata(L, val);
    lua_settable(L, index);
    return *this;
  }

  TableValProxy<K> &set(int val_index) {
    int index = m_index > 0 ? index : lua_gettop(L) + index;
    pushKey();
    lua_pushvalue(L, val_index);
    lua_settable(L, index);
    return *this;
  }

  template <class V> TableValProxy<K> &operator=(V val) { return set(val); }

  bool isNil() {
    get();
    auto res = lua_isnil(L, -1);
    lua_pop(L, 1);
    return static_cast<bool>(res);
  }

  operator lua_Number() {
    get();
    auto val = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator bool() {
    get();
    int val = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return static_cast<bool>(val);
  }

  operator std::string() {
    get();
    std::size_t len;
    const char *val = lua_tolstring(L, -1, &len);
    std::string str(val, len);
    lua_pop(L, 1);
    return str;
  }

  operator lua_CFunction() {
    get();
    auto val = lua_tocfunction(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator void *() {
    get();
    auto val = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator lua_Integer() {
    get();
    auto val = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator std::optional<lua_Number>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    auto val = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator std::optional<bool>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    int val = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return static_cast<bool>(val);
  }

  operator std::optional<std::string>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    std::size_t len;
    const char *val = lua_tolstring(L, -1, &len);
    std::string str(val, len);
    lua_pop(L, 1);
    return str;
  }

  operator std::optional<lua_CFunction>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    auto val = lua_tocfunction(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator std::optional<void *>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    auto val = lua_touserdata(L, -1);
    lua_pop(L, 1);
    return val;
  }

  operator std::optional<lua_Integer>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    auto val = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return val;
  }

private:
  lua_State *L;
  int m_index;
  K key;
};

} // namespace lege::lua

#endif
