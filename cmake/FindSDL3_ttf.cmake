#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET SDL3_ttf::SDL3_ttf)
  set(SDL3_ttf_FIND_QUIETLY TRUE)
  set(SDL3_ttf_FOUND TRUE)
  return ()
endif ()

if (SDL3_ttf_INCLUDE_DIR AND SDL3_ttf_LIBRARY)
  set(SDL3_ttf_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_SDL3_ttf sdl3-ttf)
  set(SDL3_ttf_DEFINITIONS ${PC_SDL3_ttf_CFLAGS_OTHER})
endif ()

find_path(SDL3_ttf_INCLUDE_DIR
  NAMES SDL3_ttf/SDL_ttf.h
  PATHS
    ${PC_SDL3_ttf_INCLUDEDIR}
    ${PC_SDL3_ttf_INCLUDE_DIR}
  )

find_library(SDL3_ttf_LIBRARY
  NAMES SDL3_ttf
  PATHS
    ${PC_SDL3_ttf_LIBDIR}
    ${PC_SDL3_ttf_LIBRARY_DIRS}
  )

mark_as_advanced(SDL3_ttf_INCLUDE_DIR SDL3_ttf_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL3_ttf REQUIRED_VARS SDL3_ttf_LIBRARY SDL3_ttf_INCLUDE_DIR)

if (SDL3_ttf_FOUND)
  set(SDL3_ttf_INCLUDE_DIRS "${SDL3_ttf_INCLUDE_DIR}")
  set(SDL3_ttf_LIBRARIES "${SDL3_ttf_LIBRARY}")
endif ()

if (SDL3_ttf_FOUND AND NOT TARGET SDL3_ttf::SDL3_ttf)
  add_library(SDL3_ttf::SDL3_ttf UNKNOWN IMPORTED)
  set_target_properties(SDL3_ttf::SDL3_ttf PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SDL3_ttf_INCLUDE_DIR}")
  set_target_properties(SDL3_ttf::SDL3_ttf PROPERTIES IMPORTED_LOCATION "${SDL3_ttf_LIBRARY}")
endif ()
