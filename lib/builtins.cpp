#include "builtins.hpp"

// Byte-compiled Lua modules
#include "modules/lmod_c_libs.h"

namespace lege::modules {

void register_builtins(EngineImpl &e) {
  e.load((const char *)luaJIT_BC_c_libs, luaJIT_BC_c_libs_SIZE, "b",
         "lege.c_libs");
  e.load(luaopen_lege_log, "lege.log");
  e.load(luaopen_lege_enum, "lege.enum");
  e.load(luaopen_lege_readonly, "lege.readonly");
  e.load(luaopen_lege_strict, "lege.strict");
  e.load(luaopen_lege_struct, "lege.struct");
  e.load(luaopen_lege_weak, "lege.weak");
  e.load(luaopen_utf8, "utf8");
}

} // namespace lege::modules
