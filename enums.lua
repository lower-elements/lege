--- Demonstrates the @{lege.enum} module.
-- Enums are useful when you need to store a pre-defined set of values or options.

local enum = require "lege.enum"

-- Define an enum
local Color = enum 'Color' {
    -- Strings with numeric keys define enum values
    "red",
    "green",
    "blue",
    -- String keys allow you to define a different value
    dark_orange = "red",
    -- Values don't have to be strings, but keys do
    black = 0,
}

print(Color) --> enum Color: 0xdeadbeaf

assert(Color.red == "red")
assert(Color.green == "green")
assert(Color.blue == "blue")
assert(Color.dark_orange == "red")
assert(Color.black == 0)

-- Trying to access an undefined enum value is an error
-- print(Color.white) --> Error: no value 'white' on enum 'Color'
-- Likewise trying to set a value is an error
-- Color.White = 255 --> Error: attempt to set value 'White' on enum 'Color'
