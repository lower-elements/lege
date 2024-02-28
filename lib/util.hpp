#ifndef LIBLEGE_UTIL_HPP
#define LIBLEGE_UTIL_HPP

#include <cstdint>

namespace lege {

void setSDLLogPriority();

static inline void *align_down(void *ptr, std::uintptr_t alignment) {
  return (void *)((std::uintptr_t)ptr & ~(alignment - 1));
}

static inline void *align_up(void *ptr, std::uintptr_t alignment) {
  return (void *)((std::uintptr_t)align_down(ptr, alignment) + alignment);
}

} // namespace lege

#endif
