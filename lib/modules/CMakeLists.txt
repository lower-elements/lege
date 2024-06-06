include(ByteCompileLuaModule)

foreach(module c_libs)
    add_lua_module(lege ${module} ${module}.lua)
endforeach()
