--- Demonstrates the creation of strict structs with the @{lege.struct} module
-- Strict structs have predefined fields, and trying to access undefined fields is an error.

local struct = require "lege.struct"

-- Define a Point struct, to hold x and y coordinates
local Point = struct 'Point' {
    x = 0,
    y = 0,
}

-- Create a Point
local p = Point {
    x = 10,
    y = 20,
}
print(p) --> struct Point: 0xdeadbeaf
assert(p.x == 10)
assert(p.y == 20)
-- Trying to access or modify an undefined field is an error
-- print(p.z) --> Error: no field 'z' on struct Point

-- When constructing a new struct, unspecified fields get their default values
p = Point {x=1}
assert(p.x == 1)
assert(p.y == 0)

-- Struct fields can be modified as you'd expect, but don't set them to nil!
p.y = 42
assert(p.y == 42)
