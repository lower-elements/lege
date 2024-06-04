# Locate phonon (the SteamAudio library)
# Portions of this CMake code was taken from the SteamAudio source, whos copyright notice is reproduced below:
# https://github.com/ValveSoftware/steam-audio
#
# Copyright 2017-2023 Valve Corporation.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# From: steamaudio/core/CMakeLists.txt:
#
# SYSTEM INTROSPECTION
#

# OS detection
if (WIN32)
    set(IPL_OS_WINDOWS TRUE)
elseif (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(IPL_OS_LINUX TRUE)
elseif (APPLE AND CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(IPL_OS_MACOS TRUE)
elseif (ANDROID)
    set(IPL_OS_ANDROID TRUE)
elseif (APPLE AND (CMAKE_SYSTEM_NAME STREQUAL "iOS" OR CMAKE_SYSTEM_NAME STREQUAL "tvOS" OR CMAKE_SYSTEM_NAME STREQUAL "visionOS"))
    set(IPL_OS_IOS TRUE)
    if (CMAKE_XCODE_ATTRIBUTE_SDKROOT STREQUAL "iphonesimulator")
        set(IPL_IOS_SIMULATOR TRUE)
    endif()
endif()

# CPU architecture detection
if (IPL_OS_WINDOWS)
    if (CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64")
        set(IPL_CPU_ARMV8 TRUE)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(IPL_CPU_X64 TRUE)
    else()
        set(IPL_CPU_X86 TRUE)
    endif()
elseif (IPL_OS_LINUX)
    if (CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
        set(IPL_CPU_ARMV8 TRUE)
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(IPL_CPU_X64 TRUE)
    else()
        set(IPL_CPU_X86 TRUE)
    endif()
elseif (IPL_OS_MACOS)
elseif (IPL_OS_ANDROID)
    if (CMAKE_ANDROID_ARCH STREQUAL "arm")
        set(IPL_CPU_ARMV7 TRUE)
    elseif (CMAKE_ANDROID_ARCH STREQUAL "arm64")
        set(IPL_CPU_ARMV8 TRUE)
    elseif (CMAKE_ANDROID_ARCH STREQUAL "x86")
        set(IPL_CPU_X86 TRUE)
    elseif (CMAKE_ANDROID_ARCH STREQUAL "x86_64")
        set(IPL_CPU_X64 TRUE)
    endif()
elseif (IPL_OS_IOS)
    set(IPL_CPU_ARMV8 TRUE)
endif()

# From: steamaudio/core/build/SteamAudioHelpers.cmake

function (get_bin_subdir BIN_SUBDIR)
	if (IPL_OS_WINDOWS)
		if (IPL_CPU_X86)
			set(${BIN_SUBDIR} windows-x86 PARENT_SCOPE)
		elseif (IPL_CPU_X64)
			set(${BIN_SUBDIR} windows-x64 PARENT_SCOPE)
		elseif (IPL_CPU_ARMV8)
			set(${BIN_SUBDIR} windows-arm64 PARENT_SCOPE)
		endif()
	elseif (IPL_OS_LINUX)
		if (IPL_CPU_X86)
			set(${BIN_SUBDIR} linux-x86 PARENT_SCOPE)
		elseif (IPL_CPU_X64)
			set(${BIN_SUBDIR} linux-x64 PARENT_SCOPE)
		elseif (IPL_CPU_ARMV8)
			set(${BIN_SUBDIR} linux-arm64 PARENT_SCOPE)
		endif()
	elseif (IPL_OS_MACOS)
		set(${BIN_SUBDIR} osx PARENT_SCOPE)
	elseif (IPL_OS_ANDROID)
		if (IPL_CPU_ARMV7)
			set(${BIN_SUBDIR} android-armv7 PARENT_SCOPE)
		elseif (IPL_CPU_ARMV8)
			set(${BIN_SUBDIR} android-armv8 PARENT_SCOPE)
		elseif (IPL_CPU_X86)
			set(${BIN_SUBDIR} android-x86 PARENT_SCOPE)
		elseif (IPL_CPU_X64)
			set(${BIN_SUBDIR} android-x64 PARENT_SCOPE)
		endif()
	elseif (IPL_OS_IOS)
		if (IPL_IOS_SIMULATOR)
			set(${BIN_SUBDIR} ios_simulator PARENT_SCOPE)
		else()
			set(${BIN_SUBDIR} ios PARENT_SCOPE)
		endif()
	endif()
endfunction()

get_bin_subdir(STEAMAUDIO_BIN_SUBDIR)

FIND_PATH(STEAMAUDIO_INCLUDE_DIR phonon.h
	HINTS ENV STEAMAUDIO_DIR
	PATH_SUFFIXES include
	PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/opt/local # DarwinPorts
	/opt
)

        FIND_LIBRARY(STEAMAUDIO_LIBRARY
		NAMES phonon
		HINTS ENV STEAMAUDIO_DIR
                PATH_SUFFIXES lib/${STEAMAUDIO_BIN_SUBDIR}
	)

INCLUDE(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LUAJIT_FOUND to TRUE if
# all listed variables exist
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SteamAudio
    REQUIRED_VARS STEAMAUDIO_LIBRARY STEAMAUDIO_INCLUDE_DIR)

MARK_AS_ADVANCED(STEAMAUDIO_INCLUDE_DIR STEAMAUDIO_LIBRARY)

if (SteamAudio_FOUND)
    add_library(SteamAudio SHARED IMPORTED)
    set_target_properties(SteamAudio PROPERTIES
        IMPORTED_LOCATION   "${STEAMAUDIO_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${STEAMAUDIO_INCLUDE_DIR}"
    )
endif()
