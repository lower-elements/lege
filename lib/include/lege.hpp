#ifndef LIBLEGE_LEGE_HPP
#define LIBLEGE_LEGE_HPP

#include <cstddef>
#include <string_view>

#include "lege_export.hpp"

namespace lege {

class EngineImpl;

class LEGE_EXPORT Engine final {
public:
  Engine();
  ~Engine();

  void set(std::string_view option, std::string_view val);

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
