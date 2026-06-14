#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# Pixman_FOUND - True if libPixman was found
# Pixman_INCLUDE_DIRS - Directories containing libPixman headers
# Pixman_LIBRARIES - Libraries to link against libPixman
# Pixman_DEFINITIONS - Required compiler definitions for libPixman
#

if (TARGET Pixman::Pixman)
  set(Pixman_FIND_QUIETLY TRUE)
  set(Pixman_FOUND TRUE)
  return ()
endif ()

if (Pixman_INCLUDE_DIR AND Pixman_LIBRARY)
  set(Pixman_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_Pixman pixman-1)
  set(Pixman_DEFINITIONS ${PC_Pixman_CFLAGS_OTHER})
endif ()

find_path(Pixman_INCLUDE_DIR
  NAMES pixman.h
  PATHS
    ${PC_Pixman_INCLUDEDIR}
    ${PC_Pixman_INCLUDE_DIRS}
  PATH_SUFFIXES
    pixman-1
  )
  
find_library(Pixman_LIBRARY
  NAMES pixman-1
  PATHS
    ${PC_Pixman_LIBDIR}
    ${PC_Pixman_LIBRARY_DIRS}
  )

mark_as_advanced(Pixman_INCLUDE_DIR Pixman_LIBRARY)

if (Pixman_INCLUDE_DIR)
  file(READ "${Pixman_INCLUDE_DIR}/pixman-version.h" Pixman_VERSION_CONTENT)
  string(REGEX MATCH "#define +PIXMAN_VERSION_STRING +\"(([0-9]+)\\.([0-9]+)\\.([0-9]+))\"" _dummy "${Pixman_VERSION_CONTENT}")
  set(Pixman_VERSION       "${CMAKE_MATCH_1}")
  set(Pixman_VERSION_MAJOR "${CMAKE_MATCH_2}")
  set(Pixman_VERSION_MINOR "${CMAKE_MATCH_3}")
  set(Pixman_VERSION_PATCH "${CMAKE_MATCH_4}")
  unset(Pixman_VERSION_CONTENT)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Pixman REQUIRED_VARS Pixman_LIBRARY Pixman_INCLUDE_DIR VERSION_VAR Pixman_VERSION)
if (Pixman_FOUND)
  set(Pixman_FOUND TRUE)
endif ()

if (Pixman_FOUND)
  set(Pixman_INCLUDE_DIRS "${Pixman_INCLUDE_DIR}")
  set(Pixman_LIBRARIES "${Pixman_LIBRARY}")
endif ()

if (Pixman_FOUND AND NOT TARGET Pixman::Pixman)
  add_library(Pixman::Pixman UNKNOWN IMPORTED)
  set_target_properties(Pixman::Pixman PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Pixman_DEFINITIONS}")
  set_target_properties(Pixman::Pixman PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Pixman_INCLUDE_DIR}")
  set_target_properties(Pixman::Pixman PROPERTIES IMPORTED_LOCATION "${Pixman_LIBRARY}")
endif ()
