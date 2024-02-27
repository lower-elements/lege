/*
** $Id: lprefix.h $
** Definitions for Lua code that must come before any other header file
** See Copyright Notice in lua.h
*/

#ifndef lprefix_h
#define lprefix_h

// LEGE MODIFICATNS
#include <stddef.h>

// Newer Lua has these, but LuaJIT doesn't
#ifndef luaL_pushfail
#define luaL_pushfail(L) lua_pushnil((L))
#endif
#ifndef LUA_UNSIGNED
#include <stdint.h>
#define LUA_UNSIGNED uintptr_t
typedef LUA_UNSIGNED lua_Unsigned;
#endif
#ifndef LUAMOD_API
#define LUAMOD_API /* Empty, we don't export modules */
#endif

#define UTF8_CHAR_MAX 4
size_t lege_utf8_encode(lua_Unsigned codepoint,
                        char (*utf8_buffer)[UTF8_CHAR_MAX]);

// END OF LEGE MODIFICATIONS

/*
** Allows POSIX/XSI stuff
*/
#if !defined(LUA_USE_C89) /* { */

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE 600
#elif _XOPEN_SOURCE == 0
#undef _XOPEN_SOURCE /* use -D_XOPEN_SOURCE=0 to undefine it */
#endif

/*
** Allows manipulation of large files in gcc and some other compilers
*/
#if !defined(LUA_32BITS) && !defined(_FILE_OFFSET_BITS)
#define _LARGEFILE_SOURCE 1
#define _FILE_OFFSET_BITS 64
#endif

#endif /* } */

/*
** Windows stuff
*/
#if defined(_WIN32) /* { */

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS /* avoid warnings about ISO C functions */
#endif

#endif /* } */

#endif
