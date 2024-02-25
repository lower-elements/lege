#include <cstdlib>
#include <string_view>

#include <lege.hpp>

int main() {
  lege::Engine engine;
  std::string_view test_mod = "return 5";
  engine.load(test_mod.data(), test_mod.size(), "t", "test_mod");
  engine.loadFile("init.lua", "t", "main");
  engine.run();
  return EXIT_SUCCESS;
}
