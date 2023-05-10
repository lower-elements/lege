#ifndef LEGE_CLI_ERROR_REPORTING_H
#define LEGE_CLI_ERROR_REPORTING_H

#include <stdarg.h>
#include <stdnoreturn.h>

#include "util.h"

void warn(const char *fmt, ...) LEGE_PRINTF(1, 2);
void vwarn(const char *fmt, va_list args) LEGE_PRINTF_VARARGS(1);
void error(const char *fmt, ...) LEGE_PRINTF(1, 2);
void verror(const char *fmt, va_list args) LEGE_PRINTF_VARARGS(1);
noreturn void fatal(const char *fmt, ...) LEGE_PRINTF(1, 2);

#define fatal_cleanup(label, ...)                                              \
  do {                                                                         \
    error(__VA_ARGS__);                                                        \
    goto label;                                                                \
  } while (0)

#endif
