--- Demonstrates the @{lege.event} module's @{Event|lege.event.Event} type.
-- Events allow you to define actions to be performed when an event is triggered.

local event = require "lege.event"

-- Create a new event handler object
local h = event.Event()
print(h) --> EventHandler: 0xdeadbeaf

-- Attach a handler
-- The index of the newly added handler is stored in `x`
local x = h:push(function(...)
    print("Event was triggered with the following arguments:")
    print(...)
end)
assert(x == 1, "Indices are assigned from 1")

-- Multiple handlers can be pushed
-- the `push` method is also aliased as the `__call` metamethod, so you can use this syntax:
local y = h(function() print("Another handler") end)
assert(y == 2, "Indices are consecutive")

-- Handlers can be removed at any position, much like @{table.remove}
assert(#h == 2, "Two handlers were added")
local f = h:pop(y)
assert(type(f) == "function", "`pop` returns popped handler")
assert(#h == 1, "Handler was removed")

assert(select("#", h:pop(10)) == 0, "Trying to pop an index that doesn't exist returns no values")

-- Use the `trigger` method to trigger all handlers
-- All arguments to trigger are forwarded to each handler in order, except the event handler itself
h:trigger("a", "b")
--> Event was triggered with the following arguments:
--> a	b
