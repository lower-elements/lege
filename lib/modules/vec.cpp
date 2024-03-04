#include <initializer_list>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <lua.hpp>

#include "lua/types.hpp"

namespace lua = lege::lua;

using ElementType = float;

template <const int N>
glm::vec<N, ElementType> make_vec(const ElementType *ptr) {
  if constexpr (N == 2) {
    return glm::make_vec2(ptr);
  } else if constexpr (N == 3) {
    return glm::make_vec3(ptr);
  } else if constexpr (N == 4) {
    return glm::make_vec4(ptr);
  } else {
    static_assert(N >= 2 && N <= 4, "Only vec2, vec3 and vec4 are defined");
  }
}

template <const int N> static int l_new(lua_State *L) {
  // 1 = table of methods, 2+ = ctor args
  ElementType values[N];
  lua_Number val;
  for (int i = 0; i < N; ++i) {
    values[i] = static_cast<ElementType>(lua::opt_arg(L, i + 2, val, 0.0f));
  }

  lua::new_userdata_mt<glm::vec<N, ElementType>>(
      L, lua_upvalueindex(1), std::move(make_vec<N>(values)));
  return 1;
}

template <const int N> static int l_tostring(lua_State *L) {
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  std::string str = glm::to_string(*vec);
  lua::push(L, str);
  return 1;
}

template <const int N> static int l_add(lua_State *L) {
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 + *vec2);
  return 1;
}

template <const int N> static int l_sub(lua_State *L) {
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 - *vec2);
  return 1;
}

template <const int N> static int l_mul(lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER) {
    // Multiply by scalar
    auto scalar = (ElementType)lua_tonumber(L, 1);
    auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
    lua::new_userdata<glm::vec<N, ElementType>>(L, *vec2 * scalar);
    return 1;
  }
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  if (lua_type(L, 2) == LUA_TNUMBER) {
    // Multiply by scalar
    auto scalar = (ElementType)lua_tonumber(L, 2);
    lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 * scalar);
    return 1;
  }
  // Multiply by vector
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 * *vec2);
  return 1;
}

template <const int N> static int l_div(lua_State *L) {
  if (lua_type(L, 1) == LUA_TNUMBER) {
    // Divide by scalar
    auto scalar = (ElementType)lua_tonumber(L, 1);
    auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
    lua::new_userdata<glm::vec<N, ElementType>>(L, *vec2 / scalar);
    return 1;
  }
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  if (lua_type(L, 2) == LUA_TNUMBER) {
    // Divide by scalar
    auto scalar = (ElementType)lua_tonumber(L, 2);
    lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 / scalar);
    return 1;
  }
  // Divide by vector
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::new_userdata<glm::vec<N, ElementType>>(L, *vec1 / *vec2);
  return 1;
}

template <const int N> static int l_len(lua_State *L) {
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  lua::push(L, static_cast<lua_Number>(glm::length(*vec)));
  return 1;
}

template <const int N> static int l_dot(lua_State *L) {
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::push(L, glm::dot(*vec1, *vec2));
  return 1;
}

template <const int N> static int l_cross(lua_State *L) {
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  auto *vec3 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::new_userdata<glm::vec<N, ElementType>>(L, glm::cross(*vec1, *vec3));
  return 1;
}

template <const int N> static int l_eq(lua_State *L) {
  auto *vec1 = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  auto *vec2 = lua::check_userdata<glm::vec<N, ElementType>>(L, 2);
  lua::push(L, *vec1 == *vec2);
  return 1;
}

template <const int N> static int l_unpack(lua_State *L) {
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  for (int i = 0; i < N; ++i) {
    lua_pushnumber(L, (*vec)[i]);
  }
  return N;
}

template <const int N> static int l_to_sequence(lua_State *L) {
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  lua_createtable(L, N, 0);
  for (int i = 0; i < N; ++i) {
    lua_pushnumber(L, (*vec)[i]);
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

template <const int N> static int l_to_record(lua_State *L) {
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  lua_createtable(L, 0, N);
  if constexpr (N >= 1) {
    lua::push(L, "x");
    lua::push(L, vec->x);
    lua_rawset(L, -3);
  }
  if constexpr (N >= 2) {
    lua::push(L, "y");
    lua::push(L, vec->y);
    lua_rawset(L, -3);
  }
  if constexpr (N >= 3) {
    lua::push(L, "z");
    lua::push(L, vec->z);
    lua_rawset(L, -3);
  }
  if constexpr (N >= 4) {
    lua::push(L, "w");
    lua::push(L, vec->w);
    lua_rawset(L, -3);
  }
  return 1;
}

template <const int N> static int l_index(lua_State *L) {
  lua_settop(L, 2);
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_pushvalue(L, 2);
  lua_rawget(L, 3);
  if (!lua_isnil(L, 4)) {
    return 1;
  }
  lua_settop(L, 2);
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  std::string key;
  lua::get(L, 2, key);
  if constexpr (N >= 1) {
    if (key == "x") {
      lua::push(L, vec->x);
      return 1;
    }
  }
  if constexpr (N >= 2) {
    if (key == "y") {
      lua::push(L, vec->y);
      return 1;
    }
  }
  if constexpr (N >= 3) {
    if (key == "z") {
      lua::push(L, vec->z);
      return 1;
    }
  }
  if constexpr (N >= 4) {
    if (key == "w") {
      lua::push(L, vec->w);
      return 1;
    }
  }
  return luaL_error(L, "No field '%s' on vec%d", key.data(), N);
}

template <const int N> static int l_newindex(lua_State *L) {
  lua_settop(L, 3);
  auto *vec = lua::check_userdata<glm::vec<N, ElementType>>(L, 1);
  std::string_view key;
  lua::get(L, 2, key);
  if constexpr (N >= 1) {
    if (key == "x") {
      vec->x = (ElementType)luaL_checknumber(L, 3);
      return 0;
    }
  }
  if constexpr (N >= 2) {
    if (key == "y") {
      vec->y = (ElementType)luaL_checknumber(L, 3);
      return 0;
    }
  }
  if constexpr (N >= 3) {
    if (key == "z") {
      vec->z = (ElementType)luaL_checknumber(L, 3);
      return 0;
    }
  }
  if constexpr (N >= 4) {
    if (key == "w") {
      vec->w = (ElementType)luaL_checknumber(L, 3);
      return 0;
    }
  }
  return luaL_error(L, "No field '%s' on vec%d", key.data(), N);
}

template <const int N> int open_lege_vec(lua_State *L) {
  lua_settop(L, 0);
  lua::make_metatable<glm::vec<N, ElementType>>(L);

  // Create the table of vec methods, used for both the returned module table,
  // as well as the __index metamethod
  lua_createtable(L, 0, 3);
  // Dot product
  lua::push(L, "dot");
  lua_pushcfunction(L, l_dot<N>);
  lua_rawset(L, 2);
  if constexpr (N == 3) {
    // Cross product
    lua::push(L, "cross");
    lua_pushcfunction(L, l_cross<N>);
    lua_rawset(L, 2);
  }
  // Unpack a vec to separate coordinates
  lua::push(L, "unpack");
  lua_pushcfunction(L, l_unpack<N>);
  lua_rawset(L, 2);
  // Convert a vec to a sequence
  lua::push(L, "to_sequence");
  lua_pushcfunction(L, l_to_sequence<N>);
  lua_rawset(L, 2);
  // Convert a vec to a record
  lua::push(L, "to_record");
  lua_pushcfunction(L, l_to_record<N>);
  lua_rawset(L, 2);

  // __index metamethod, takes methods as an upvalue
  lua::push(L, "__index");
  lua_pushvalue(L, 2);
  lua::push(L, l_index<N>, 1);
  lua_rawset(L, 1);
  // __newindex metamethod, takes methods as an upvalue
  lua::push(L, "__newindex");
  lua_pushcfunction(L, l_newindex<N>);
  lua_rawset(L, 1);

  // __tostring metamethod
  lua::push(L, "__tostring");
  lua_pushcfunction(L, l_tostring<N>);
  lua_rawset(L, 1);
  // __add, __sub, __mul and __div metamethods
  lua::push(L, "__add");
  lua_pushcfunction(L, l_add<N>);
  lua_rawset(L, 1);
  lua::push(L, "__sub");
  lua_pushcfunction(L, l_sub<N>);
  lua_rawset(L, 1);
  lua::push(L, "__mul");
  lua_pushcfunction(L, l_mul<N>);
  lua_rawset(L, 1);
  lua::push(L, "__div");
  lua_pushcfunction(L, l_div<N>);
  lua_rawset(L, 1);
  // __eq metamethod
  lua::push(L, "__eq");
  lua_pushcfunction(L, l_eq<N>);
  lua_rawset(L, 1);
  // __len metamethod
  lua::push(L, "__len");
  lua_pushcfunction(L, l_len<N>);
  lua_rawset(L, 1);

  // Calling the returned methods tble acts as the constructor
  // Need another metatable to do that
  lua_createtable(L, 0, 1);
  // Vec constructor
  lua::push(L, "__call");
  lua_pushvalue(L, 1);
  lua::push(L, l_new<N>, 1);
  lua_rawset(L, 3);
  // setmetatable(methods_table, mt)
  lua_setmetatable(L, 2);

  return 1;
}

extern "C" int luaopen_lege_vec2(lua_State *L) { return open_lege_vec<2>(L); }

extern "C" int luaopen_lege_vec3(lua_State *L) { return open_lege_vec<3>(L); }

extern "C" int luaopen_lege_vec4(lua_State *L) { return open_lege_vec<4>(L); }
