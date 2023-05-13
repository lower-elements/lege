#include "util.h"
#include <stdio.h>
#include <stdlib.h>

HEDLEY_MALLOC
LEGE_ALLOC_SIZE(1) HEDLEY_RETURNS_NON_NULL void *lege_malloc(size_t size) {
  void *ptr = malloc(size);
  if (HEDLEY_UNLIKELY(!ptr)) {
    perror("malloc");
    abort();
  }
  return ptr;
}

HEDLEY_MALLOC LEGE_ALLOC_SIZE(1, 2) HEDLEY_RETURNS_NON_NULL
    void *lege_calloc(size_t nmem, size_t size) {
  void *ptr = calloc(nmem, size);
  if (HEDLEY_UNLIKELY(!ptr)) {
    perror("malloc");
    abort();
  }
  return ptr;
}

HEDLEY_MALLOC
char *lege_read_to_buf(const char *fname, size_t *bufsize) {
  FILE *fp = fopen(fname, "r");
  if (HEDLEY_UNLIKELY(!fp)) {
    return NULL;
  }
  if (HEDLEY_UNLIKELY(fseek(fp, 0, SEEK_END) != 0)) {
    fclose(fp);
    return NULL;
  }
  long pos = ftell(fp);
  if (HEDLEY_UNLIKELY(pos < 0)) {
    fclose(fp);
    return NULL;
  }
  rewind(fp);
  char *buf = lege_xmalloc(pos + 1);
  if (HEDLEY_UNLIKELY(!buf)) {
    fclose(fp);
    return NULL;
  }
  if (HEDLEY_UNLIKELY(fread(buf, 1, pos, fp) != (size_t)pos)) {
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
