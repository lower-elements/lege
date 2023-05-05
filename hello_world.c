#include <lege.h>
#include <stdio.h>

int main(void) {
  lege_engine_t engine = lege_engine_new();
  if (!engine) {
    perror("Failed to create engine");
    return 1;
  }
  lege_engine_set_string(engine, LEGE_OPTION_APP_NAME, "Hello World");
  lege_engine_set_string(engine, LEGE_OPTION_ORG_NAME, "lower-elements");
  lege_engine_load_literal(engine, "print('Hello, world!')", "init.lua");
  lege_engine_run(engine);
  lege_engine_free(engine);
  return 0;
}
