#include <SDL.h>
#include <lauxlib.h>
#include <limits.h>
#include <lua.h>
#include <luaconf.h>
#include <stdbool.h>

#include "lege_internal.h"
#include "util.h"

/**
 * LEGE event handling.
 * This module defines a generic mechanism for defining actions to be performed,
 * and triggering those actions, with the @{Event} type. It also defines
 * functions for managing the LEGE event loop. After initializing the
 * appropriate constructs, call @{main_loop} to start the engine and begin
 * processing events.
 * @module lege.event
 */

static int l_event_trigger(lua_State *L);

/**
 * Start the engine's main loop, processing events and triggering event
 * handlers.
 * @function main_loop
 */
static int l_main_loop(lua_State *L) {
  lua_settop(L, 0); // Discard all arguments
  // Get the events table
  lua_pushvalue(L, lua_upvalueindex(1));
  while (true) {
    // Handle events
    for (SDL_Event e; SDL_PollEvent(&e);) {
      switch (e.type) {
      case SDL_QUIT:
        lua_pushcfunction(L, l_event_trigger);
        lua_getfield(L, 1, "quit");
        lua_call(L, 1, 0);
        return 0;
      case SDL_APP_LOWMEMORY:
        lua_gc(L, LUA_GCCOLLECT, 0);
        break;
      }
    }
  }
}

static const char *const EVENT_MT_NAME = "lege.event.Event";

/**
 * Convert an @{Event} to a human-readable string.
 * Currently we merely output "Event" and its address.
 * @meta Event.__tostring
 * @tparam Event self The Event to convert to a string
 * @treturn string A formatted string "Event: &lt;address&gt;"
 */
static int l_event_tostring(lua_State *L) {
  const void *ptr = lua_topointer(L, 1);
  lua_pushfstring(L, "Event: %p", ptr);
  return 1;
}

/**
 * Attach a handler to an @{Event}.
 * This inserts the handler at the end of the event table, like @{table.insert}.
 * @usage
 * local h = Event()
 * local index1 = h:push(function() print("Event triggered") end)
 * -- Push is also aliased as the __call metamethod
 *  local index2 = h(function() print("Another handler") end)
 * @function Event.push
 * @tparam Event self The event handler to operate on
 * @param handler The handler to push
 * @treturn number The index of the newly pushed handler, which can later be
 * passed to @{Event.pop}
 */
static int l_event_push(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checkany(L, 2); // Any callable
  // Todo: Possibly implement varargs to push multiple handlers at
  // once
  lua_settop(L, 2); // Discard any further values

  // Insert the handler at the end of the list
  size_t new_idx = lua_objlen(L, 1) + 1;
  SDL_assert(new_idx < (size_t)LUA_MAXINTEGER);
  lua_rawseti(L, 1, (lua_Integer)new_idx);

  // Return the index so you can remove the handler later if you
  // wish
  lua_pushnumber(L, (lua_Number)new_idx);
  return 1;
}

/**
 * Pop a handler from the @{Event} at the given index.
 * All other handlers are shifted down to fill the gap, just like
@{table.remove}.
 * @function Event.pop
 * @tparam Event self The event handler to operate on
 * @tparam number index The index of the handler to pop
 * @return[1] The popped handler object
 * @return[2] No values, if the index was out of range
 */
static int l_event_pop(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  int idx = luaL_checkint(L, 2);
  // Todo: Possibly implement varargs to pop multiple handlers at
  // once
  lua_settop(L, 2); // Discard any further values

  // Save the handler at the current index to return later (will be
  // at index 3)
  lua_rawgeti(L, 1, idx);
  if (lua_isnil(L, 3)) {
    return 0; // Return no values
  }

  // Loop through the table indices, shifting each one down, until
  // we hit nil
  bool keep_going = true;
  do {
    lua_rawgeti(L, 1, idx + 1);
    if (lua_isnil(L, 4)) {
      keep_going = false;
    }
    lua_rawseti(L, 1, idx++);
  } while (keep_going);

  // Return the popped handler
  lua_settop(L, 3);
  return 1;
}

/**
 * Trigger an @{Event}, calling all attached handlers.
 * Handlers are called in the order they're attached. All arguments passed to
 * this function after `self` will be forwarded to each handler.
 * @function Event.trigger
 * @tparam Event self The event handler to trigger
 * @param ... Additional arguments to pass to handlers
 */
static int l_event_trigger(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  int top = lua_gettop(L);

  // Loop through each handler
  lua_Number handler_idx = 1;
  while (true) {
    lua_rawgeti(L, 1, handler_idx);
    if (lua_isnil(L, -1)) {
      break; // End of handlers
    }
    // Copy all arguments to pass to the handler
    for (int i = 2; i <= top; ++i) {
      lua_pushvalue(L, i);
    }
    // Call the handler
    lua_call(L, top - 1, 0);

    ++handler_idx; // Next handler
  }
  return 0;
}

/**
 * Create a new event.
 * @usage
 * local event = require "lege.event"
 *
 * -- Create a new event handler object
 * local h = event.Event()
 * print(h) --> EventHandler: 0xdeadbeaf
 *
 * -- Attach a handler
 * -- The index of the newly added handler is stored in `x`
 * local x = h:push(function(...)
 * print("Event was triggered with the following arguments:")
 * print(...)
 * end)
 * assert(x == 1, "Indices are assigned from 1")
 *
 * -- Multiple handlers can be pushed
 * -- the `push` method is also aliased as the `__call` metamethod, so you can
 * use this syntax: local y = h(function() print("Another handler") end)
 * assert(y == 2, "Indices are consecutive")
 *
 * -- Handlers can be removed at any position, much like @{table.remove}
 * assert(#h == 2, "Two handlers were added")
 * local f = h:pop(y)
 * assert(type(f) == "function", "`pop` returns popped handler")
 * assert(#h == 1, "Handler was removed")
 *
 * assert(select("#", h:pop(10)) == 0, "Trying to pop an index that doesn't
 * exist returns no values")
 *
 * -- Use the `trigger` method to trigger all handlers
 * -- All arguments to trigger are forwarded to each handler in order, except
 * the event handler itself h:trigger("a", "b")
 * --> Event was triggered with the following arguments:
 * --> a	b
 * @function Event
 * @return An Event object
 */
static int l_event(lua_State *L) {
  lua_newtable(L);
  lua_getfield(L, LUA_REGISTRYINDEX, EVENT_MT_NAME);
  lua_setmetatable(L, -2);
  return 1;
}

static const char *const EVENT_NAMES[] = {"quit", NULL};

static void make_events_table(lua_State *L) {
  lua_createtable(L, 0, arraysize(EVENT_NAMES) - 1);
  for (const char *const *name = EVENT_NAMES; *name; ++name) {
    // Create a new Event type
    l_event(L); // Event is at stack top
    lua_setfield(L, -2, *name);
  }
}

int luaopen_lege_event(lua_State *L) {
  // Initialize the SDL event subsystem
  if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
    return luaL_error(L, "Failed to initialize event subsystem: %s",
                      SDL_GetError());
  }
  lege_engine_t engine = ll_get_engine(L);
  engine->initialized_sdl_subsystems |= SDL_INIT_EVENTS;

  // Create mt for Event type
  if (luaL_newmetatable(L, EVENT_MT_NAME)) {
    lua_pushliteral(L, "Event");
    lua_setfield(L, 1, "__name");
    lua_pushvalue(L, 1);
    lua_setfield(L, 1, "__index");
    lua_pushcfunction(L, l_event_tostring);
    lua_setfield(L, 1, "__tostring");
    lua_pushcfunction(L, l_event_push);
    lua_pushvalue(L, -1);
    lua_setfield(L, 1, "__call");
    lua_setfield(L, 1, "push");
    lua_pushcfunction(L, l_event_pop);
    lua_setfield(L, 1, "pop");
    lua_pushcfunction(L, l_event_trigger);
    lua_setfield(L, 1, "trigger");
  }

  // Create and populate the event table
  lua_createtable(L, 0, 3);
  make_events_table(L);
  lua_pushvalue(L, -1); // Duplicate for upvalue of l_main_loop
  lua_setfield(L, 2, "on");
  lua_pushcclosure(L, l_main_loop, 1);
  lua_setfield(L, 2, "main_loop");
  lua_pushcfunction(L, l_event);
  lua_setfield(L, 2, "Event");

  // Return the event table
  return 1;
}
