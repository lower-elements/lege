#ifndef LIBLEGE_LEGE_HPP
#define LIBLEGE_LEGE_HPP

#include <cstddef>

#include "lege_export.hpp"

namespace lege {

class EngineImpl;

class LEGE_EXPORT Engine final {
public:
  Engine();
  ~Engine();

  void load(const char *buf, std::size_t size, const char *mode = "t",
            const char *name = "main");

  void setup();
  [[nodiscard]] bool runOnce();
  void run();

private:
  EngineImpl *m_impl;
};

} // namespace lege

#endif
