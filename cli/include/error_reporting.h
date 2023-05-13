#ifndef LEGE_CLI_ERROR_REPORTING_H
#define LEGE_CLI_ERROR_REPORTING_H

#include <hedley.h>
#include <stdarg.h>
#include <stdnoreturn.h>

#include "util.h"

void warn(const char *fmt, ...) HEDLEY_PRINTF_FORMAT(1, 2);
void vwarn(const char *fmt, va_list args) HEDLEY_PRINTF_FORMAT(1, 0);
void error(const char *fmt, ...) HEDLEY_PRINTF_FORMAT(1, 2);
void verror(const char *fmt, va_list args) HEDLEY_PRINTF_FORMAT(1, 0);
noreturn void fatal(const char *fmt, ...) HEDLEY_PRINTF_FORMAT(1, 2);

#define fatal_cleanup(label, ...)                                              \
  do {                                                                         \
    error(__VA_ARGS__);                                                        \
    goto label;                                                                \
  } while (0)

#endif
