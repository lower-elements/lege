# LEGE - The Lower Elements Game Engine

[![License: LGPL3](https://img.shields.io/github/license/lower-elements/lege)][lgpl3]
[![Documentation](https://img.shields.io/badge/documentation-view%20Online-brightgreen)][docs]
[![GitHub issues](https://img.shields.io/github/issues/lower-elements/lege)][issues]

[lgpl3]: <https://www.gnu.org/licenses/lgpl-3.0.en.html>
[docs]: <https://lower-elements.github.io/lege>
[issues]: <https://github.com/lower-elements/lege/issues>

LEGE is a [free software][fs], [open source][os] game engine for building [audio games][ag]. Its goals, in order, are to
be:

1. [Easy to use](#easy-to-use)
2. [Cross platform](#cross-platform)
3. [Powerful and extensible](#powerful-and-extensable)
4. [Blazing fast!](#blazing-fast)

[fs]: <https://en.wikipedia.org/wiki/Free_software>
[os]: <https://en.wikipedia.org/wiki/Open_source>
[ag]: https://en.wikipedia.org/wiki/Audio_game<https://en.wikipedia.org/wiki/Audio_game>

## Under Active and Early Development

This software is under active development and is in an early state. Everything described in this readme is aspirational,
and not necessarily representative of LEGE as it stands today. If you'd like to help, we love getting [pull
requests][pr].

[pr]: <https://github.com/lower-elements/lege/pull/new>

## Easy to Use

LEGE uses the [Lua programming language][lua]: "a powerful, efficient, lightweight, embeddable scripting language." It
is easy to get started building LEGE games, even if you're quite new to programming. The knowledge you gain writing LEGE
applications in Lua can also be used to write standalone Lua applications, and we embrace and promote Lua best practices
and existing convensions.

[lua]: <https://lua.org>

## Cross Platform

LEGE leverages well established, portable C libraries including [SDL][sdl] for input and rendering, and [OpenAL][openal]
(specifically [OpenAL Soft][openal-soft]) for high quality, 3 dimensional audio. It is compatible with all major desktop
OS platforms: Windows, macOS, Linux, \*BSD, etc. I also plan to put effort into making LEGE work on mobile devices where
possible, such as on Android and iOS. Audio gaming should be accessible to all!

Distributing a LEGE project is as easy as running `lege dist`, and publishing the built distribution (.zip / .exe
installer on Windows, .app / .dmg on macOS, Flatpak / Snap / Deb / whatever package format you choose on Linux).

[sdl]: <https://libsdl.org>
[openal]: <https://openal.org>
[openal-soft]: <https://openal-soft.org>

## Powerful and Extensible

LEGE is designed to be highly extensible, offering the flexibility to use existing Lua or C modules, as well as
third-party providers for additional engine functionalities like map loading, networking, game chat, and updates. You
can even create your own modules to enhance the engine's capabilities. 

Want to use the Discord SDK in your game? It's as easy as:

```sh
lege package add discord-sdk
```

```lua
-- file: init.lua
local discord = require "discord-sdk"
```

# Blazing Fast!

LEGE utilizes [LuaJIT][luajit] by default, which employs just-in-time compilation to translate Lua code into native
machine code, resulting in performance nearly on par with C. The core engine is written in C to ensure optimal
performance. Our aim is to strike the perfect balance between speed and ease of use.

[luajit]: <https://luajit.org>


## Documentation

View the LEGE manual, API documentation, and examples on [Github Pages][docs]

## License

LEGE is distributed under the terms of the [GNU Lesser General Public License 3.0][lgpl3]:

Copyright (C) 2023  Michael Connor Buchan <mikey@blindcomputing.org>

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; version 3 of the License.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, see
<https://www.gnu.org/licenses/>.
