#ifndef LIBLEGE_UTIL_HPP
#define LIBLEGE_UTIL_HPP

#include <cstdint>
#include <cstdlib>
#include <cxxabi.h>
#include <exception>
#include <memory>
#include <type_traits>
#include <typeindex>

#include <fmt/core.h>

namespace lege {

static inline void *align_down(void *ptr, std::uintptr_t alignment) {
  return (void *)((std::uintptr_t)ptr & ~(alignment - 1));
}

static inline void *align_up(void *ptr, std::uintptr_t alignment) {
  return (void *)((std::uintptr_t)align_down(ptr, alignment) + alignment);
}

template <class T> std::size_t type_id() {
  return std::type_index(typeid(T)).hash_code();
}

template <class T>
std::unique_ptr<char[], void (*)(void *)>
demangled_name(std::size_t *size = nullptr) {
  const char *mangled_name = typeid(T).name();
  int status;
  char *res = abi::__cxa_demangle(mangled_name, nullptr, size, &status);
  switch (status) {
  case 0:
    return {res, std::free};
  case -1:
    throw std::bad_alloc();
  case -2:
    throw std::runtime_error(
        fmt::format("Invalid mangled type name: {}", mangled_name));
  case -3:
    throw std::invalid_argument("Invalid argument to __cxa_demangle");
  default:
    throw std::runtime_error(
        fmt::format("Could not demangle type name: {}", mangled_name));
  }
}

} // namespace lege

#endif
