add_library(lege-rt STATIC
    lua/error.cpp
    lua/stack.cpp
    lua/state.cpp
    lua/table_view.cpp
    modules/weak.cpp
    runtime.cpp
    util.cpp
    )

set_property(TARGET lege-rt PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(lege-rt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lege-rt liblua-shared)
