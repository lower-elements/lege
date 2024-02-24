add_library(lege SHARED engine.cpp)

target_include_directories(lege PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(lege PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(lege PROPERTIES CXX_VISIBILITY_PRESET hidden)
set_target_properties(lege PROPERTIES VISIBILITY_INLINES_HIDDEN 1)
include(GenerateExportHeader)
generate_export_header(lege
    EXPORT_FILE_NAME lege_export.hpp
    DEFINE_NO_DEPRECATED
    )

target_link_libraries(lege PRIVATE fmt glm::glm OpenAL::OpenAL SDL2 SDL2::SDL2)

install(TARGETS lege DESTINATION ${LIBRARY_INSTALL_DIR})
install(FILES
    include/lege.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/lege_export.hpp
    DESTINATION include/lege
    )