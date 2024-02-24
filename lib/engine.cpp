#include <cstdlib>

#include <SDL.h>

#include "engine.hpp"
#include "lege.hpp"

namespace lege {

Engine::Engine() : m_impl(new EngineImpl()) {}

Engine::~Engine() { delete m_impl; }

int Engine::run() { return EXIT_SUCCESS; }

} // namespace lege
