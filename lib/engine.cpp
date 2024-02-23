#include <SDL.h>

#include "lege.hpp"

namespace lege {

Engine::Engine() { SDL_Init(SDL_INIT_VIDEO); }

Engine::~Engine() { SDL_Quit(); }

} // namespace lege
