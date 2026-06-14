#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# Cairo_FOUND - True if libcairo was found
# Cairo_INCLUDE_DIRS - Directories containing libcairo headers
# Cairo_LIBRARIES - Libraries to link against libcairo
# Cairo_DEFINITIONS - Required compiler definitions for libcairo
#

if (TARGET Cairo::Cairo)
  set(Cairo_FIND_QUIETLY TRUE)
  set(Cairo_FOUND TRUE)
  return ()
endif ()

if (Cairo_INCLUDE_DIR AND Cairo_LIBRARY)
  set(Cairo_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_Cairo cairo)
  set(Cairo_DEFINITIONS ${PC_Cairo_CFLAGS_OTHER})
endif ()

find_path(Cairo_INCLUDE_DIR
  NAMES cairo.h
  PATHS
    ${PC_Cairo_INCLUDEDIR}
    ${PC_Cairo_INCLUDE_DIRS}
  PATH_SUFFIXES
    cairo
  )

find_library(Cairo_LIBRARY
  NAMES cairo
  PATHS
    ${PC_Cairo_LIBDIR}
    ${PC_Cairo_LIBRARY_DIRS}
  )

mark_as_advanced(Cairo_INCLUDE_DIR Cairo_LIBRARY)

if (Cairo_INCLUDE_DIR)
  file(READ "${Cairo_INCLUDE_DIR}/cairo-version.h" Cairo_VERSION_CONTENT)

  string(REGEX MATCH "#define +CAIRO_VERSION_MAJOR +([0-9]+)" _dummy "${Cairo_VERSION_CONTENT}")
  set(Cairo_VERSION_MAJOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +CAIRO_VERSION_MINOR +([0-9]+)" _dummy "${Cairo_VERSION_CONTENT}")
  set(Cairo_VERSION_MINOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +CAIRO_VERSION_MICRO +([0-9]+)" _dummy "${Cairo_VERSION_CONTENT}")
  set(Cairo_VERSION_PATCH "${CMAKE_MATCH_1}")

  set(Cairo_VERSION "${Cairo_VERSION_MAJOR}.${Cairo_VERSION_MINOR}.${Cairo_VERSION_PATCH}")
  unset(Cairo_VERSION_CONTENT)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Cairo REQUIRED_VARS Cairo_LIBRARY Cairo_INCLUDE_DIR VERSION_VAR Cairo_VERSION)
if (CAIRO_FOUND)
  set(Cairo_FOUND TRUE)
endif ()

if (Cairo_FOUND)
  set(Cairo_INCLUDE_DIRS ${Cairo_INCLUDE_DIR})
  set(Cairo_LIBRARIES ${Cairo_LIBRARY})
endif ()

if (Cairo_FOUND AND NOT TARGET Cairo::Cairo)
  add_library(Cairo::Cairo UNKNOWN IMPORTED)
  set_target_properties(Cairo::Cairo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Cairo_INCLUDE_DIR}")
  set_target_properties(Cairo::Cairo PROPERTIES IMPORTED_LOCATION "${Cairo_LIBRARY}")
  set_target_properties(Cairo::Cairo PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Cairo_DEFINITIONS}")
endif ()
