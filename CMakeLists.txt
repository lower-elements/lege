cmake_minimum_required(VERSION 3.14)

project(LEGE
    VERSION 0.1.0
    DESCRIPTION "The Lower Elements Game Engine"
    LANGUAGES C CXX
    )

# User-writable options

# Global project options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Dependencies
find_package(SteamAudio REQUIRED)

include(FetchContent)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG 10.2.1
  GIT_SHALLOW ON
  FIND_PACKAGE_ARGS NAMES fmt
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.1
  GIT_SHALLOW ON
  FIND_PACKAGE_ARGS NAMES glm
)

FetchContent_Declare(
    LuaJIT
    GIT_REPOSITORY https://github.com/9chu/LuaJIT-cmake
    GIT_SHALLOW ON
    FIND_PACKAGE_ARGS
)

FetchContent_Declare(
  SDL2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL/
  GIT_TAG release-2.30.3
  GIT_SHALLOW ON
  URL_HASH MD5=a344eb827a03045c9b399e99af4af13d
  TLS_VERIFY true
  DOWNLOAD_EXTRACT_TIMESTAMP true
  FIND_PACKAGE_ARGS NAMES SDL2 COMPONENTS SDL2main
)

FetchContent_MakeAvailable(fmt glm LuaJIT SDL2)
if (LuaJIT_FOUND)
    add_library(liblua-shared INTERFACE)
    target_include_directories(liblua-shared INTERFACE ${LUA_INCLUDE_DIR})
    target_link_libraries(liblua-shared INTERFACE ${LUA_LIBRARY})
endif()

# Build liblege and the lege command line utility
add_subdirectory(engine)
add_subdirectory(rt)
add_subdirectory(lib)
add_subdirectory(cli)
