--- Access to common C libraries.
-- Libraries used by LEGE, such as [SDL](https://libsdl.org) are exposed here as loaded FFI libraries.
-- @module lege.c_libs
-- @see luajit:ext_ffi

local ffi = require "ffi"


-- --- The returned libraries
-- @table
return {
    --- The [SDL library](https://libsdl.org).
    -- Used by LEGE for graphics, input, and operating system functions such as file handling.
    --
    -- LEGE currently uses SDL2.
    -- @field sdl
    sdl = ffi.load("SDL2"),
}
