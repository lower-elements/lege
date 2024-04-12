-- ldoc configuration file

project = "LEGE"
package = "lib"

title = "LEGE Documentation"
description = "The Lower Elements Game Engine"
full_description = [[
### ⚠️ Unstable APIs Ahead!

This is a beta (I.E. before v1.0.0) version of LEGE. As such, all APIs are considered unstable unless otherwise noted.
]]

format = "markdown"
topics = {"README.md"}
file = {"lib", "rt"}
examples = "examples"

new_type("meta", "Meta Methods", false, "param")
custom_see_handler('^sdl:([%w_]*)$', function(sdl_symbol)
  return sdl_symbol, "https://wiki.libsdl.org/SDL2/" .. sdl_symbol
end)
custom_see_handler('^luajit:([%w_]*)$', function(page)
  return page, "https://luajit.org/" .. page .. ".html"
end)
