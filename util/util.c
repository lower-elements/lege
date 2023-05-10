#include "util.h"
#include <stdio.h>
#include <stdlib.h>

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

char *lege_read_to_buf(const char *fname, size_t *bufsize) {
  FILE *fp = fopen(fname, "r");
  if (UNLIKELY(!fp)) {
    return NULL;
  }
  if (UNLIKELY(fseek(fp, 0, SEEK_END) != 0)) {
    fclose(fp);
    return NULL;
  }
  long pos = ftell(fp);
  if (UNLIKELY(pos < 0)) {
    fclose(fp);
    return NULL;
  }
  rewind(fp);
  char *buf = lege_xmalloc(pos + 1);
  if (UNLIKELY(!buf)) {
    fclose(fp);
    return NULL;
  }
  if (UNLIKELY(fread(buf, 1, pos, fp) != pos)) {
    fclose(fp);
    lege_free(buf);
    return NULL;
  }
  fclose(fp);
  buf[pos] = '\0';
  if (bufsize) {
    *bufsize = pos;
  }
  return buf;
}
