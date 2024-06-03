#include <exception>
#include <memory>
#include <string_view>

#include <SDL_rwops.h>
#include <fmt/core.h>
#include <lua.hpp>

#include "builtins.hpp"
#include "engine.hpp"
#include "lege.hpp"
#include "lua/helpers.hpp"
#include "sdl/helpers.hpp"

namespace lua = lege::lua;
namespace sdl = lege::sdl;

namespace lege {

Engine::Engine() : m_impl(new EngineImpl) {}

Engine::~Engine() { delete m_impl; }

void Engine::set(std::string_view option, std::string_view val) {
  m_impl->set(option, val);
}

void Engine::load(const char *buf, std::size_t size, const char *mode,
                  const char *name) {
  m_impl->load(buf, size, mode, name);
}

void Engine::loadFile(const char *filename, const char *mode,
                      const char *name) {
  m_impl->loadFile(filename, mode, name);
}

void Engine::loadProject(const char *projectfile) {
  m_impl->loadProject(projectfile);
}

void Engine::setup() { m_impl->setup(); }

bool Engine::runOnce() { return m_impl->runOnce(); }

void Engine::run() {
  m_impl->setup();
  while (m_impl->runOnce()) {
  }
}

EngineImpl::EngineImpl() : GameEngine(), Runtime() {}

EngineImpl::~EngineImpl() {}

void EngineImpl::set(std::string_view option, std::string_view val) {
  // Get the options table
  luaL_newmetatable(L, "lege.options");
  lua::push(L, option);
  lua::push(L, val);
  lua_rawset(L, -3);
  lua_pop(L, 1);
}

void EngineImpl::loadFile(const char *filename, const char *mode,
                          const char *name) {
  // Todo: investigate using mmap() here
  std::size_t sz;
  auto contents = (char *)SDL_LoadFile(filename, &sz);
  if (!contents) {
    throw sdl::Error(fmt::format("could not load chunk \"{}\"", filename));
  }
  // We do this instead of manual memory management to avoid leaking memory if
  // load() throws an exception
  std::unique_ptr<char[], void (*)(void *)> buf(contents, SDL_free);
  load(buf.get(), sz, mode, name);
}

void EngineImpl::loadProject(const char *projectfile) {
  // Use an isolated, temporary environment:
  lua_newtable(L);

  int res = luaL_loadfilex(L, projectfile, "t");
  if (res != LUA_OK) {
    lua_replace(L, -2); // Replace environment table to keep stack balanced
    throw lua::Error(L,
                     fmt::format("Could not load project manifest from \"{}\"",
                                 projectfile));
  }
  lua_pushvalue(L, -2); // Duplicate env table
  // -3 = env table, -2 = chunk, -1 = env table
  lua_setfenv(L, -2);
  // -2 = env table, -1 = chunk
  // Execute the project manifest
  lua_call(L, 0, 0);

  // Set options
  lua_getfield(L, -1, "options");
  if (lua_istable(L, -1)) {
    int options_tbl = lua_gettop(L);
    std::string option;
    for (lua_pushnil(L); lua_next(L, options_tbl); lua_pop(L, 1)) {
      std::string_view key, val;
      lua::get(L, -2, key);
      lua::get(L, -1, val);
      // Prepend "lege." to all engine options so future extensions can have
      // other namespaces
      option = "lege.";
      option += key;
      SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
                   "Setting option \"%s\" to \"%.*s\"", option.c_str(),
                   (int)val.size(), val.data());
      set(option, val);
    }
  }
  lua_pop(L, 1); // Pop options table

  // Load modules
  lua_getfield(L, -1, "modules");
  if (!lua_istable(L, -1)) {
    throw std::runtime_error(fmt::format(
        "Expected `modules` to be a table, got {}", luaL_typename(L, -1)));
  }
  int modules_tbl = lua_gettop(L);
  std::string mod_name, file;
  for (lua_pushnil(L); lua_next(L, modules_tbl); lua_pop(L, 1)) {
    lua::get(L, -1, file);
    if (lua_isnumber(L, -2)) {
      // Module name is the file name, sans ".lua"
      std::size_t dot = std::string::npos;
      if (file.ends_with(".lua")) {
        dot = file.rfind('.');
      }
      if (dot != std::string::npos) {
        mod_name = file.substr(0, dot);
      } else {
        mod_name = file;
      }
    } else if (lua_isstring(L, -2)) {
      // Key is the module name
      lua::get(L, -2, mod_name);
    } else {
      throw std::runtime_error(
          fmt::format("Invalid type for key of `modules` table: expected "
                      "string or number, got {}",
                      luaL_typename(L, -2)));
    }
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION,
                 "Loading module \"%s\" from file \"%s\"", mod_name.c_str(),
                 file.c_str());
    loadFile(file.c_str(), "t", mod_name.c_str());
  }
  lua_pop(L, 1);

  // Pop the environment table
  lua_pop(L, 1);
}

void EngineImpl::setup() {
  lege::modules::register_builtins(*this);
  Runtime::setup();
}

} // namespace lege
