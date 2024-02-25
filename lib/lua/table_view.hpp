#ifndef LIBLEGE_LUA_TABLE_VIEW_HPP
#define LIBLEGE_LUA_TABLE_VIEW_HPP

#include <optional>
#include <string>
#include <string_view>

#include <lua.hpp>

#include "lua/stack.hpp"

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
  void get() {
    int index = m_index >= 0 ? index : lua_gettop(L) + index;
    push(key);
    lua_gettable(L, index);
  }

  void get_raw() {
    int index = m_index >= 0 ? index : lua_gettop(L) + index;
    push(key);
    lua_rawget(L, index);
  }

  template <class V> TableValProxy<K> &set(V val) {
    int index = absindex(L, m_index);
    push(key);
    push(val);
    lua_settable(L, index);
    return *this;
  }

  template <class V> TableValProxy<K> &set_raw(V val) {
    int index = absindex(L, m_index);
    push(key);
    push(val);
    lua_rawset(L, index);
    return *this;
  }

  template <class V> TableValProxy<K> &operator=(V val) { return set(val); }

  bool isNil() {
    get();
    auto res = lua_isnil(L, -1);
    lua_pop(L, 1);
    return static_cast<bool>(res);
  }

  template <class V> operator V() {
    get();
    V val;
    ::lege::lua::get(L, val);
    lua_pop(L, 1);
    return val;
  }

  template <class V> operator std::optional<V>() {
    get();
    if (lua_isnil(L, -1)) {
      return {};
    }
    V val;
    ::lege::lua::get(L, val);
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
