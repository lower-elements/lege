--- The "lege.strict" module prevents use of undeclared global variables.
-- Since all undeclared global variables in Lua have the value nil by default, it can be easy to accidentally use a
-- global variable when you really wanted to use a local variable. You might even accidentally overwrite a global
-- variable by typoing the name of an existing local variable. The "lege.strict" module causes accesses to global
-- variables outside of C or a module's main chunk (I.E. not in any function) to raise an error.

require "lege.strict"

-- With strict, accessing global variables from anywhere that aren't first declared is an error
-- print("The value of x is " .. tostring(x)) -- Error: variable 'x' is not declared

-- You declare global variables anywhere in a main chunk (I.E. not in a function)
-- They can be set to anything, even nil!
x = nil
print("The value of x is " .. tostring(x)) --> The value of x is nil
