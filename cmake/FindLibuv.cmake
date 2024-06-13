# Locate libuv (cross-platform asynchronous I/O)

find_package(PkgConfig)

if (PKG_CONFIG_FOUND)
    pkg_check_modules(UV libuv QUIET IMPORTED_TARGET)
endif()

if (UV_FOUND)
    add_library(uv ALIAS PkgConfig::UV)
endif()

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set UV_FOUND to TRUE if
# all listed variables exist
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Libuv
    REQUIRED_VARS UV_LIBRARIES)

MARK_AS_ADVANCED(UV_INCLUDE_DIR UV_LIBRARY)
