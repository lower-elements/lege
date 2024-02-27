#include <cstddef>

#include <SDL_error.h>

#include "sdl/error.hpp"

namespace lege::sdl {

Error::Error(std::string prefix) {
  const char *err = SDL_GetError();
  m_msg = prefix;
  m_msg += ": ";
  m_msg += err;
}

Error::~Error() {}

const char *Error::what() const noexcept { return m_msg.c_str(); }

} // namespace lege::sdl
