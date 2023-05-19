-- ldoc configuration file

project = "LEGE"
package = "lege"

title = "LEGE Documentation"
description = "The Lower Elements Game Engine"
format = "markdown"

topics = {"README.md"}
file = "lege"
examples = "examples"

new_type("meta", "Meta Methods", false, "param")
custom_see_handler('^sdl:([%w_]*)$', function(sdl_symbol)
  return sdl_symbol, "https://wiki.libsdl.org/SDL2/" .. sdl_symbol
end)
