#include <cstddef>

#include <SDL.h>
#include <lua.h>

extern "C" {
#include "modules/lprefix.h" // For UTF8_CHAR_MAX
}

#include "util.hpp"

/**
 * Used by lib/modules/lutf8lib.c (LEGE port of PUC Lua 5.4's utf8 module)
 */
extern "C" std::size_t lege_utf8_encode(lua_Unsigned codepoint,
                                        char (*utf8_buffer)[UTF8_CHAR_MAX]) {
  if (codepoint <= 0x7F) {
    (*utf8_buffer)[0] = (char)codepoint;
    return 1;
  } else if (codepoint <= 0x7FF) {
    (*utf8_buffer)[0] = (char)(0xC0 | (codepoint >> 6));
    (*utf8_buffer)[1] = (char)(0x80 | (codepoint & 0x3F));
    return 2;
  } else if (codepoint <= 0xFFFF) {
    (*utf8_buffer)[0] = (char)(0xE0 | (codepoint >> 12));
    (*utf8_buffer)[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
    (*utf8_buffer)[2] = (char)(0x80 | (codepoint & 0x3F));
    return 3;
  } else if (codepoint <= 0x10FFFF) {
    (*utf8_buffer)[0] = (char)(0xF0 | (codepoint >> 18));
    (*utf8_buffer)[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
    (*utf8_buffer)[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
    (*utf8_buffer)[3] = (char)(0x80 | (codepoint & 0x3F));
    return 4;
  }
  return 0; // Invalid codepoint
}

namespace lege {

static const char *const LOG_PRIORITIES[]{"verbose", "debug", "info",
                                          "warn",    "error", "critical"};

void setSDLLogPriority() {
  const char *log_env = SDL_getenv("LEGE_LOG");

  if (log_env) {
    for (unsigned i = 0; i < SDL_arraysize(LOG_PRIORITIES); ++i) {
      if (SDL_strcasecmp(log_env, LOG_PRIORITIES[i]) == 0) {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,
                           (SDL_LogPriority)(i + 1));
        return;
      }
    }
    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "Unknown value for LEGE_LOG: '%s'", log_env);
  }
}

} // namespace lege
