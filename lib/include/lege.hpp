#ifndef LIBLEGE_LEGE_HPP
#define LIBLEGE_LEGE_HPP

#include "lege_export.hpp"

namespace lege {

class EngineImpl;

class LEGE_EXPORT Engine final {
public:
  Engine();
  ~Engine();

  int run();

private:
  EngineImpl *m_impl;
};

} // namespace lege

#endif
