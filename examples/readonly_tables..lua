--- Demonstrates the @{lege.readonly} module, to make tables readonly.
-- Readonly tables can be useful for constant values, to prevent code from accidentally modifying them.

local readonly = require "lege.readonly"

local consts = readonly {1, 2, 3; a=42, b=1337}

-- Accessing values works as normal
print("The value of consts.b is " .. consts.b)
--> The value of b is 1337

-- But changing them is an error
-- consts.b = 100
--> Error: attempt to modify readonly table

-- You also can't get or set the table's metatable
print("consts' metatable is " .. tostring(getmetatable(consts)))
--> consts' metatable is false
-- setmetatable(consts, {})
--> Error: cannot change a protected metatable
