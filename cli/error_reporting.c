#include <stdio.h>
#include <stdlib.h>

#include "args.h"
#include "error_reporting.h"

void vwarn(const char *fmt, va_list args) {
  fprintf(stderr, "%s: Warning: ", PROG_NAME);
  vfprintf(stderr, fmt, args);
  putc('\n', stderr);
}

void warn(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vwarn(fmt, args);
  va_end(args);
}

void verror(const char *fmt, va_list args) {
  fprintf(stderr, "%s: Error: ", PROG_NAME);
  vfprintf(stderr, fmt, args);
  putc('\n', stderr);
}

void error(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  verror(fmt, args);
  va_end(args);
}

void fatal(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  verror(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}
