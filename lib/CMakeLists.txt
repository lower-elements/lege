add_library(lege SHARED
    builtins.cpp
    engine.cpp
    modules/enum.cpp
    modules/log.cpp
    modules/lutf8lib.c
    modules/readonly.cpp
    modules/strict.cpp
    modules/struct.cpp
    modules/window.cpp
    modules/vec.cpp
    )

target_compile_definitions(lege PRIVATE -DGLM_ENABLE_EXPERIMENTAL)

target_include_directories(lege PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(lege PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})

set_target_properties(lege PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR})
set_target_properties(lege PROPERTIES CXX_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN 1)

include(GenerateExportHeader)
generate_export_header(lege
    EXPORT_FILE_NAME lege_export.hpp
    DEFINE_NO_DEPRECATED
    )

target_link_libraries(lege PRIVATE lege-engine lege-rt fmt glm::glm liblua-shared SDL2 SDL2::SDL2)

install(TARGETS lege DESTINATION ${LIBRARY_INSTALL_DIR})
install(FILES
    include/lege.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/lege_export.hpp
    DESTINATION include/lege
    )

add_subdirectory(modules)
