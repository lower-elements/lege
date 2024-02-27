# Lua modules:
# These are byte-compiled and embedded in liblege, then preloaded by the library
function(add_lua_module MODULE_NAME SOURCE_FILE)
    find_program(LUAJIT luajit)
    if(NOT LUAJIT)
        message(FATAL_ERROR "LuaJIT not found")
    endif()

    set(luajit_compile_opts "-b" "-t" "h")

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        list(APPEND luajit_compile_opts "-g")
    else()
        list(APPEND luajit_compile_opts "-s")
    endif()

    set(OUTPUT_HEADER "lmod_${MODULE_NAME}.h")
    
    add_custom_command(
        OUTPUT ${OUTPUT_HEADER}
        COMMAND ${LUAJIT}
        ARGS ${luajit_compile_opts} "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_HEADER}"
        DEPENDS ${SOURCE_FILE}
        COMMENT "Byte-compiling Lua module to ${OUTPUT_HEADER}"
    )
    
add_custom_target(lmod_${MODULE_NAME} DEPENDS ${OUTPUT_HEADER})
add_dependencies(lege lmod_${MODULE_NAME})
endfunction()

foreach(module c_libs)
    add_lua_module(${module} ${module}.lua)
endforeach()
