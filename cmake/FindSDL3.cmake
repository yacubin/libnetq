#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET SDL3::SDL3)
  set(SDL3_FIND_QUIETLY TRUE)
  set(SDL3_FOUND TRUE)
  return ()
endif ()

if (SDL3_INCLUDE_DIR AND SDL3_LIBRARY)
  set(SDL3_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_SDL3 sdl3)
  set(SDL3_DEFINITIONS ${PC_SDL3_CFLAGS_OTHER})
endif ()

find_path(SDL3_INCLUDE_DIR
  NAMES SDL3/SDL.h
  PATHS
    ${PC_SDL3_INCLUDEDIR}
    ${PC_SDL3_INCLUDE_DIR}
  )

find_library(SDL3_LIBRARY
  NAMES SDL3
  PATHS
    ${PC_SDL3_LIBDIR}
    ${PC_SDL3_LIBRARY_DIRS}
  )

mark_as_advanced(SDL3_INCLUDE_DIR SDL3_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL3 DEFAULT_MSG SDL3_LIBRARY SDL3_INCLUDE_DIR)

if (SDL3_FOUND)
  set(SDL3_INCLUDE_DIRS ${SDL3_INCLUDE_DIR})
  set(SDL3_LIBRARIES ${SDL3_LIBRARY})
endif ()

if (SDL3_FOUND AND NOT TARGET SDL3::SDL3)
  add_library(SDL3::SDL3 UNKNOWN IMPORTED)
  set_target_properties(SDL3::SDL3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}")
  set_target_properties(SDL3::SDL3 PROPERTIES IMPORTED_LOCATION "${SDL3_LIBRARY}")
endif ()
