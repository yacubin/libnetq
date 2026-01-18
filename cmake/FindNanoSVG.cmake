#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

set(NanoSVG_INCLUDE_DIRS "")
set(NanoSVG_LIBRARIES "")
set(NanoSVG_PACKAGE_HANDLE_STANDARD_ARGS "")

if (NOT TARGET NanoSVG::nanosvg)
  find_path(NanoSVG_nanosvg_INCLUDE_DIR
    NAMES nanosvg.h
    PATH_SUFFIXES nanosvg
    )

  find_library(NanoSVG_nanosvg_LIBRARY
    NAMES nanosvg
    )

  mark_as_advanced(
    NanoSVG_nanosvg_INCLUDE_DIR
    NanoSVG_nanosvg_LIBRARY
    )

  add_library(NanoSVG::nanosvg INTERFACE IMPORTED)
  set_property(TARGET NanoSVG::nanosvg PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${NanoSVG_nanosvg_INCLUDE_DIR}")
  set_property(TARGET NanoSVG::nanosvg PROPERTY INTERFACE_LINK_LIBRARIES "${NanoSVG_nanosvg_LIBRARY}")

  list(APPEND NanoSVG_INCLUDE_DIRS "${NanoSVG_nanosvg_INCLUDE_DIR}")
  list(APPEND NanoSVG_LIBRARIES "${NanoSVG_nanosvg_LIBRARY}")
  list(APPEND NanoSVG_PACKAGE_HANDLE_STANDARD_ARGS NanoSVG_nanosvg_INCLUDE_DIR NanoSVG_nanosvg_LIBRARY)
endif ()

if (NOT TARGET NanoSVG::nanosvgrast)
  find_path(NanoSVG_nanosvgrast_INCLUDE_DIR
    NAMES nanosvgrast.h
    PATH_SUFFIXES nanosvg
    )
  find_library(NanoSVG_nanosvgrast_LIBRARY
    NAMES nanosvgrast
    )
  mark_as_advanced(
    NanoSVG_nanosvgrast_INCLUDE_DIR
    NanoSVG_nanosvgrast_LIBRARY
    )
  add_library(NanoSVG::nanosvgrast INTERFACE IMPORTED)
  set_property(TARGET NanoSVG::nanosvgrast PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${NanoSVG_nanosvgrast_INCLUDE_DIR}")
  set_property(TARGET NanoSVG::nanosvgrast PROPERTY INTERFACE_LINK_LIBRARIES "${NanoSVG_nanosvgrast_LIBRARY}")

  list(APPEND NanoSVG_INCLUDE_DIRS "${NanoSVG_nanosvgrast_INCLUDE_DIR}")
  list(APPEND NanoSVG_LIBRARIES "${NanoSVG_nanosvgrast_LIBRARY}")
  list(APPEND NanoSVG_PACKAGE_HANDLE_STANDARD_ARGS NanoSVG_nanosvgrast_INCLUDE_DIR NanoSVG_nanosvgrast_LIBRARY)
endif ()

if (NanoSVG_PACKAGE_HANDLE_STANDARD_ARGS)
  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(NanoSVG DEFAULT_MSG ${NanoSVG_PACKAGE_HANDLE_STANDARD_ARGS})
endif ()

if (NANOSVG_FOUND OR NanoSVG_FOUND)
  set(NanoSVG_FOUND TRUE)
endif()

if (NanoSVG_FOUND AND NOT TARGET NanoSVG::NanoSVG)
  add_library(NanoSVG::NanoSVG INTERFACE IMPORTED)
  set_target_properties(NanoSVG::NanoSVG PROPERTIES INTERFACE_LINK_LIBRARIES "NanoSVG::nanosvg;NanoSVG::nanosvgrast")
endif ()
