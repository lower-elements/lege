/**
 * Shows how to preload Lua modules into a LEGE engine.
 */

#include <lege.h>
#include <stdio.h>

static const char module[] = "\
                              print('Test module loaded')\
                              return {\
                                  x = 5,\
                                      y = 10,\
                              }\
                              ";

static const char entrypoint[] = "\
                                   local mod = require('test_module')\
                                   print(mod.x + mod.y)\
                                   ";

int main(void) {
  lege_engine_t engine = lege_engine_new();
  if (!engine) {
    perror("Failed to create engine");
    return 1;
  }
  lege_engine_set_string(engine, LEGE_OPTION_APP_NAME, "Hello World");
  lege_engine_set_string(engine, LEGE_OPTION_ORG_NAME, "lower-elements");
  lege_engine_preload_literal(engine, module, "test_module");
  lege_engine_load_literal(engine, entrypoint, "init.lua");
  lege_engine_run(engine);
  lege_engine_free(engine);
  return 0;
}
