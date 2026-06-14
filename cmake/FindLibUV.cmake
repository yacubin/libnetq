#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# LibUV_FOUND - True if libuv was found
# LibUV_INCLUDE_DIRS - Directories containing libuv headers
# LibUV_LIBRARIES - Libraries to link against libuv
# LibUV_DEFINITIONS - Required compiler definitions for libuv
#

if (TARGET LibUV::LibUV)
  set(LibUV_FIND_QUIETLY TRUE)
  set(LibUV_FOUND TRUE)
  return ()
endif ()

if (LibUV_INCLUDE_DIR AND LibUV_LIBRARY)
  set(LibUV_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibUV libuv)
  set(LibUV_DEFINITIONS ${PC_LibUV_CFLAGS_OTHER})
endif ()

find_path(LibUV_INCLUDE_DIR
  NAMES uv.h
  PATHS
    ${PC_LibUV_INCLUDEDIR}
    ${PC_LibUV_INCLUDE_DIRS}
  PATH_SUFFIXES
    libuv
  )
  
find_library(LibUV_LIBRARY
  NAMES uv
  PATHS
    ${PC_LibUV_LIBDIR}
    ${PC_LibUV_LIBRARY_DIRS}
  )

mark_as_advanced(LibUV_INCLUDE_DIR LibUV_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibUV REQUIRED_VARS LibUV_LIBRARY LibUV_INCLUDE_DIR)
if (LIBUV_FOUND)
  set(LibUV_FOUND TRUE)
endif ()

if (LibUV_FOUND)
  set(LibUV_INCLUDE_DIRS "${LibUV_INCLUDE_DIR}")
  set(LibUV_LIBRARIES "${LibUV_LIBRARY}")
endif ()

if (LibUV_FOUND AND NOT TARGET LibUV::LibUV)
  add_library(LibUV::LibUV UNKNOWN IMPORTED)
  set_target_properties(LibUV::LibUV PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${LibUV_DEFINITIONS}")
  set_target_properties(LibUV::LibUV PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibUV_INCLUDE_DIR}")
  set_target_properties(LibUV::LibUV PROPERTIES IMPORTED_LOCATION "${LibUV_LIBRARY}")
endif ()
