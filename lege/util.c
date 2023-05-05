#include <stdio.h>
#include <stdlib.h>

#include "lege.h"
#include "util.h"

LEGE_MALLOC LEGE_ALLOC_SIZE(1) LEGE_RETURNS_NONNULL
    LEGE_DEALLOC(lege_free) void *lege_malloc(size_t size) {
  void *ptr = malloc(size);
  if (UNLIKELY(!ptr)) {
    perror("malloc");
    abort();
  }
  return ptr;
}

LEGE_MALLOC LEGE_ALLOC_SIZE2(1, 2) LEGE_RETURNS_NONNULL
    LEGE_DEALLOC(lege_free) void *lege_calloc(size_t nmem, size_t size) {
  void *ptr = calloc(nmem, size);
  if (UNLIKELY(!ptr)) {
    perror("malloc");
    abort();
  }
  return ptr;
}
