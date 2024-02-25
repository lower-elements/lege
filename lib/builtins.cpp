#include "builtins.hpp"

namespace lege::modules {

void register_builtins(EngineImpl &e) {
  e.load(luaopen_lege_log, "lege.log");
  e.load(luaopen_lege_enum, "lege.enum");
  e.load(luaopen_lege_readonly, "lege.readonly");
  e.load(luaopen_lege_strict, "lege.strict");
  e.load(luaopen_lege_struct, "lege.struct");
  e.load(luaopen_lege_weak, "lege.weak");
}

} // namespace lege::modules
