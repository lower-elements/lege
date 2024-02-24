#include <cstdlib>
#include <string_view>

#include <lege.hpp>

int main() {
  lege::Engine engine;
  std::string_view test_mod = "return 5";
  engine.load(test_mod.data(), test_mod.size(), "t", "test_mod");
  std::string_view code = "local t = require 'test_mod'; print(t)";
  engine.load(code.data(), code.size(), "t");
  engine.run();
  return EXIT_SUCCESS;
}
