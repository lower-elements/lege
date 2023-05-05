#include <stdbool.h>
#include <stdio.h>

#include "lege.h"
#include "util.h"

struct lege_engine {
  const char *app_name;
  const char *org_name;
};

lege_engine_t lege_engine_new(void) {
  lege_engine_t engine = lege_xnew(struct lege_engine);
  if (LIKELY(engine)) {
    engine->app_name = "LEGE Application";
    engine->org_name = "lower-elements";
  }
  return engine;
}

void lege_engine_free(lege_engine_t engine) { lege_free(engine); }

bool lege_engine_set_string(lege_engine_t engine, lege_option_t opt,
                            const char *str) {
  switch (opt) {
  case LEGE_OPTION_APP_NAME:
    engine->app_name = str;
    return true;
  case LEGE_OPTION_ORG_NAME:
    engine->org_name = str;
    return true;
  default:
    return false; // Option not recognised
  }
}

void lege_engine_run(lege_engine_t engine) {
  printf("Starting %s.%s\n", engine->org_name, engine->app_name);
}
