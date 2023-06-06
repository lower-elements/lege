#ifndef LIBLEGE_UTIL_H
#define LIBLEGE_UTIL_H

#include <SDL_stdinc.h>
#include <hedley.h>
#include <stdlib.h>

#if HEDLEY_HAS_ATTRIBUTE(alloc_size)
#define LEGE_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#define LEGE_ALLOC_SIZE(...)
#endif

#if HEDLEY_HAS_BUILTIN(__builtin_types_compatible_p)
#define assert_types_compatible(name, t1, t2)                                  \
  SDL_COMPILE_TIME_ASSERT(name, __builtin_types_compatible_p(t1, t2))
#else
#define assert_types_compatible(name, t1, t2)                                  \
  SDL_COMPILE_TIME_ASSERT(name, sizeof(t1) == sizeof(t2) &&                    \
                                    _Alignof(t1) == _Alignof(t2))
#endif

// Cross-compiler constructor functions
// https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc
// 2010-2016 Joe Lowe. Released into the public domain.
#ifdef __cplusplus
#define INITIALIZER(f)                                                         \
  static void f(void);                                                         \
  struct f##_t_ {                                                              \
    f##_t_(void) { f(); }                                                      \
  };                                                                           \
  static f##_t_ f##_;                                                          \
  static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU", read)
#define INITIALIZER2_(f, p)                                                    \
  static void f(void);                                                         \
  __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f;                     \
  __pragma(comment(linker, "/include:" p #f "_")) static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f, "")
#else
#define INITIALIZER(f) INITIALIZER2_(f, "_")
#endif
#else
#define INITIALIZER(f)                                                         \
  static void f(void) __attribute__((constructor));                            \
  static void f(void)
#endif

#define arraysize(a) (sizeof(a) / sizeof((a)[0]))
#define strempty(s) (*(s) == '\0')

// Memory allocation
// Functions / macros with an `x` return NULL on failure, the others abort().
#define lege_xmalloc malloc
#define lege_xrealloc realloc
#define lege_free free
void *lege_malloc(size_t size) HEDLEY_MALLOC
    LEGE_ALLOC_SIZE(1) HEDLEY_RETURNS_NON_NULL;
void *lege_calloc(size_t nmem, size_t size) HEDLEY_MALLOC
    LEGE_ALLOC_SIZE(1, 2) HEDLEY_RETURNS_NON_NULL;
void *lege_realloc(void *ptr, size_t size)
    LEGE_ALLOC_SIZE(2) HEDLEY_RETURNS_NON_NULL;
#define lege_new(type) ((type *)lege_malloc(sizeof(type)))
#define lege_xnew(type) ((type *)lege_malloc(sizeof(type)))

// I/O helpers
char *lege_read_to_buf(const char *fname, size_t *bufsize) HEDLEY_MALLOC;

// Lua helpers
// ll = LEGE Lua
#define ll_for_each_pair(L, T)                                                 \
  for (lua_pushnil((L)); lua_next((L), (T)) != 0; lua_pop((L), 1))
#define ll_rawset_cfunc(L, tbl, key, val)                                      \
  do {                                                                         \
    lua_pushliteral((L), key);                                                 \
    lua_pushcfunction((L), (val));                                             \
    lua_rawset((L), (tbl));                                                    \
  } while (0)

// Newer Lua has these, but LuaJIT doesn't, so add them for compatibility, in
// case we want to allow alternative Lua implementations (fairly unlikely)
#ifndef luaL_pushfail
#define luaL_pushfail(L) lua_pushnil((L))
#endif
#ifndef LUA_UNSIGNED
#include <stdint.h>
#define LUA_UNSIGNED uintptr_t
typedef LUA_UNSIGNED lua_Unsigned;
#endif
#ifndef LUAMOD_API
#define LUAMOD_API /* Empty, we don't export modules */
#endif
#ifndef LUA_MAXINTEGER
// I think this is correct, at least for all two's complement machines which
// accounts for basically everything LEGE runs on, and assuming that lua_Integer
// is a signed type?
#define LUA_MAXINTEGER ((~(lua_Integer)0) - 1)
#endif

// UTF8 helpers
#define UTF8_CHAR_MAX 4
size_t lege_utf8_encode(lua_Unsigned codepoint,
                        char (*utf8_buffer)[UTF8_CHAR_MAX]);

#endif
