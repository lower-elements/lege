cmake_minimum_required(VERSION 3.14)

project(LEGE
    VERSION 0.1.0
    DESCRIPTION "The Lower Elements Game Engine"
    LANGUAGES CXX
    )

# User-writable options

# Global project options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Dependencies
include(FetchContent)

FetchContent_Declare(
  fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/10.2.0.tar.gz
  URL_HASH MD5=683182450c06031d2611d29e1773f3ba
  TLS_VERIFY true
  DOWNLOAD_EXTRACT_TIMESTAMP true
  FIND_PACKAGE_ARGS NAMES fmt
)

FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip
  URL_HASH MD5=69895110052f0d711c9c54fbf385f6f5
  TLS_VERIFY true
  DOWNLOAD_EXTRACT_TIMESTAMP true
  FIND_PACKAGE_ARGS NAMES glm
)

set(ALSOFT_UTILS OFF CACHE INTERNAL "ALSOFT_UTILS")
set(ALSOFT_NO_CONFIG_UTIL ON CACHE INTERNAL "ALSOFT_NO_CONFIG_UTIL")
set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "ALSOFT_EXAMPLES")
FetchContent_Declare(
    OpenAL
	URL https://github.com/kcat/openal-soft/archive/refs/tags/1.23.1.tar.gz
        URL_HASH MD5=190a60118236644b78488ccab26ebfdf
	TLS_VERIFY ON
	DOWNLOAD_EXTRACT_TIMESTAMP true
        FIND_PACKAGE_ARGS
        )

FetchContent_Declare(
  SDL2
  URL https://libsdl.org/release/SDL2-2.28.5.tar.gz
  URL_HASH MD5=a344eb827a03045c9b399e99af4af13d
  TLS_VERIFY true
  DOWNLOAD_EXTRACT_TIMESTAMP true
  FIND_PACKAGE_ARGS NAMES SDL2 COMPONENTS SDL2main
)

FetchContent_MakeAvailable(fmt glm OpenAL SDL2)

# Build liblege
add_subdirectory(lib)
