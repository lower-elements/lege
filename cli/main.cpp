#include <cstdlib>
#include <string_view>

#include <lege.hpp>

int main() {
  lege::Engine engine;
  engine.loadProject("project.lua");
  engine.run();
  return EXIT_SUCCESS;
}
