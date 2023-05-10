#ifndef LIBLEGE_UTIL_H
#define LIBLEGE_UTIL_H

#include <stdlib.h>

/**
 * Check if the compiler supports a certain attribute / builtin.
 * Compilers that lack __has_attribute may object to
 *     #if defined __has_attribute && __has_attribute (...)
 * even though they do not need to evaluate the right-hand side of the &&.
 * Similarly for __has_builtin, etc.
 * Taken from glibc's sys/cdefs.h
 */
#if (defined __has_attribute &&                                                \
     (!defined __clang_minor__ ||                                              \
      3 < __clang_major__ + (5 <= __clang_minor__)))
#define LEGE_HAS_ATTRIBUTE(attr) __has_attribute(attr)
#else
#define LEGE_HAS_ATTRIBUTE(attr) 0
#endif
#ifdef __has_builtin
#define LEGE_HAS_BUILTIN(name) __has_builtin(name)
#else
#define LEGE_HAS_BUILTIN(name) 0
#endif

#if LEGE_HAS_BUILTIN(__builtin_expect)
#define LIKELY(x) __builtin_expect((long)x, 1)
#define UNLIKELY(x) __builtin_expect((long)x, 0)
#else
#define LIKELY(x) x
#define UNLIKELY(x) x
#endif

// Attributes
#if LEGE_HAS_ATTRIBUTE(alloc_size)
#define LEGE_ALLOC_SIZE(pos) __attribute__((alloc_size(pos)))
#define LEGE_ALLOC_SIZE2(p1, p2) __attribute__((alloc_size(p1, p2)))
#else
#define LEGE_ALLOC_SIZE(pos)
#define LEGE_ALLOC_SIZE2(p1, p2)
#endif
#if LEGE_HAS_ATTRIBUTE(malloc)
#define LEGE_MALLOC __attribute__((malloc))
#define LEGE_DEALLOC(func) __attribute__((malloc(func)))
#else
#define LEGE_MALLOC
#define LEGE_DEALLOC(func)
#endif
#if LEGE_HAS_ATTRIBUTE(returns_nonnull)
#define LEGE_RETURNS_NONNULL __attribute__((returns_nonnull))
#else
#define LEGE_RETURNS_NONNULL
#endif

#if LEGE_HAS_ATTRIBUTE(format)
#define LEGE_PRINTF(str_idx, args_idx)                                         \
  __attribute__((format(printf, (str_idx), (args_idx))))
#define LEGE_PRINTF_VARARGS(str_idx)                                           \
  __attribute__((format(printf, (str_idx), 0)))
#else
#define LEGE_PRINTF(str_idx, args_idx)
#define LEGE_PRINTF_VARARGS(str_idx)
#endif

// Memory allocation
// Functions / macros with an `x` return NULL on failure, the others abort().
#define lege_xmalloc malloc
#define lege_free free
void *lege_malloc(size_t size) LEGE_MALLOC
    LEGE_ALLOC_SIZE(1) LEGE_RETURNS_NONNULL LEGE_DEALLOC(lege_free);
void *lege_calloc(size_t nmem, size_t size) LEGE_MALLOC
    LEGE_ALLOC_SIZE2(1, 2) LEGE_RETURNS_NONNULL LEGE_DEALLOC(lege_free);
#define lege_new(type) ((type *)lege_malloc(sizeof(type)))
#define lege_xnew(type) ((type *)lege_malloc(sizeof(type)))

// I/O helpers
char *lege_read_to_buf(const char *fname, size_t *bufsize) LEGE_MALLOC
    LEGE_DEALLOC(lege_free);

// Lua helpers
// ll = LEGE Lua
#define ll_for_each_pair(L, T)                                                 \
  for (lua_pushnil((L)); lua_next((L), (T)) != 0; lua_pop((L), 1))

#endif
