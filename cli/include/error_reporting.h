#ifndef LEGE_CLI_ERROR_REPORTING_H
#define LEGE_CLI_ERROR_REPORTING_H

#include <stdarg.h>
#include <stdnoreturn.h>

void warn(const char *fmt, ...);
void vwarn(const char *fmt, va_list args);
void error(const char *fmt, ...);
void verror(const char *fmt, va_list args);
noreturn void fatal(const char *fmt, ...);

#define fatal_cleanup(label, ...)                                              \
  do {                                                                         \
    error(__VA_ARGS__);                                                        \
    goto label;                                                                \
  } while (0)

#endif
