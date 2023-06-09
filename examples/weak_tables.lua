--- Demonstrates the @{lege.weak} module, for convenient creation of weak tables.

local weak = require "lege.weak"

-- Construct a table with both weak keys and weak values ('kv')
local t = weak 'kv' {}
print(t) --> weak table kv: 0xdeadbeaf
print(getmetatable(t).__mode) --> kv

-- For more information about weak tables, see section 2.10.2 of the Lua 5.1 reference manual:
-- <https://www.lua.org/manual/5.1/manual.html#2.10.2>
