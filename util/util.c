#include <stdio.h>
#include <stdlib.h>

#include "util.h"

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

size_t lege_utf8_encode(lua_Unsigned codepoint,
                        char (*utf8_buffer)[UTF8_CHAR_MAX]) {
  if (codepoint <= 0x7F) {
    (*utf8_buffer)[0] = (char)codepoint;
    return 1;
  } else if (codepoint <= 0x7FF) {
    (*utf8_buffer)[0] = (char)(0xC0 | (codepoint >> 6));
    (*utf8_buffer)[1] = (char)(0x80 | (codepoint & 0x3F));
    return 2;
  } else if (codepoint <= 0xFFFF) {
    (*utf8_buffer)[0] = (char)(0xE0 | (codepoint >> 12));
    (*utf8_buffer)[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
    (*utf8_buffer)[2] = (char)(0x80 | (codepoint & 0x3F));
    return 3;
  } else if (codepoint <= 0x10FFFF) {
    (*utf8_buffer)[0] = (char)(0xF0 | (codepoint >> 18));
    (*utf8_buffer)[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
    (*utf8_buffer)[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
    (*utf8_buffer)[3] = (char)(0x80 | (codepoint & 0x3F));
    return 4;
  }
  return 0; // Invalid codepoint
}
