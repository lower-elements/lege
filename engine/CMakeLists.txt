add_library(lege-engine STATIC
    sdl/error.cpp
    sdl/helpers.cpp
    )

set_property(TARGET lege-engine PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(lege-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lege-engine PRIVATE fmt glm::glm SDL2 SDL2::SDL2)
