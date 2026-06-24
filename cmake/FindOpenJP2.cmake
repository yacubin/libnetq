#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET OpenJP2::OpenJP2)
  set(OpenJP2_FIND_QUIETLY TRUE)
  set(OpenJP2_FOUND TRUE)
  return ()
endif ()

if (OpenJP2_INCLUDE_DIR AND OpenJP2_LIBRARY)
  set(OpenJP2_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_OpenJP2 openjp2)
endif ()

find_path(OpenJP2_INCLUDE_DIR
  NAMES openjpeg.h
  PATHS
    ${PC_OpenJP2_INCLUDEDIR}
    ${PC_OpenJP2_INCLUDE_DIR}
  PATH_SUFFIXES
    openjpeg-2.5
  )

find_library(OpenJP2_LIBRARY
  NAMES openjp2
  PATHS
    ${PC_OpenJP2_LIBDIR}
    ${PC_OpenJP2_LIBRARIES_DIRS}
  )

mark_as_advanced(OpenJP2_INCLUDE_DIR OpenJP2_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenJP2 REQUIRED_VARS OpenJP2_LIBRARY OpenJP2_INCLUDE_DIR)
if (OPENJP2_FOUND)
  set(OpenJP2_FOUND TRUE)
endif()

if (OpenJP2_FOUND)
  set(OpenJP2_INCLUDE_DIRS "${OpenJP2_INCLUDE_DIR}")
  set(OpenJP2_LIBRARIES "${OpenJP2_LIBRARY}")
endif()

if (OpenJP2_FOUND AND NOT TARGET OpenJP2::OpenJP2)
  add_library(OpenJP2::OpenJP2 UNKNOWN IMPORTED)
  set_target_properties(OpenJP2::OpenJP2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${OpenJP2_INCLUDE_DIR}")
  set_target_properties(OpenJP2::OpenJP2 PROPERTIES IMPORTED_LOCATION "${OpenJP2_LIBRARY}")
endif ()
