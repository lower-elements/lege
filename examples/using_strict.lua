require "lege.strict"

-- With strict, accessing global variables from anywhere that aren't first declared is an error
-- print("The value of x is " .. tostring(x)) -- Error: variable 'x' is not declared

-- You declare global variables anywhere in a main chunk (I.E. not in a function)
-- They can be set to anything, even nil!
x = nil
print("The value of x is " .. tostring(x)) --> The value of x is nil
