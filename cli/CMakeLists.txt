add_executable(lege-bin main.cpp)
set_target_properties(lege-bin PROPERTIES OUTPUT_NAME lege)
target_link_libraries(lege-bin PUBLIC lege)
