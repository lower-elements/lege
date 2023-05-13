#ifndef LIBLEGE_UTIL_H
#define LIBLEGE_UTIL_H

#include <hedley.h>
#include <stdlib.h>

#if HEDLEY_HAS_ATTRIBUTE(alloc_size)
#define LEGE_ALLOC_SIZE(...) __attribute__((alloc_size(__VA_ARGS__)))
#else
#define LEGE_ALLOC_SIZE(...)
#endif

// Memory allocation
// Functions / macros with an `x` return NULL on failure, the others abort().
#define lege_xmalloc malloc
#define lege_free free
void *lege_malloc(size_t size) HEDLEY_MALLOC
    LEGE_ALLOC_SIZE(1) HEDLEY_RETURNS_NON_NULL;
void *lege_calloc(size_t nmem, size_t size) HEDLEY_MALLOC
    LEGE_ALLOC_SIZE(1, 2) HEDLEY_RETURNS_NON_NULL;
#define lege_new(type) ((type *)lege_malloc(sizeof(type)))
#define lege_xnew(type) ((type *)lege_malloc(sizeof(type)))

// I/O helpers
char *lege_read_to_buf(const char *fname, size_t *bufsize) HEDLEY_MALLOC;

// Lua helpers
// ll = LEGE Lua
#define ll_for_each_pair(L, T)                                                 \
  for (lua_pushnil((L)); lua_next((L), (T)) != 0; lua_pop((L), 1))

#endif
