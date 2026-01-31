#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET AmiVideo::AmiVideo)
  set(AmiVideo_FIND_QUIETLY TRUE)
  set(AmiVideo_FOUND TRUE)
  return ()
endif ()

if (AmiVideo_INCLUDE_DIR AND AmiVideo_LIBRARY)
  set(AmiVideo_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_AmiVideo libamivideo)
endif ()

find_path(AmiVideo_INCLUDE_DIR
  NAMES libamivideo/amivideotypes.h
  PATHS
    ${PC_AmiVideo_INCLUDEDIR}
    ${PC_AmiVideo_INCLUDE_DIR}
  )

find_library(AmiVideo_LIBRARY
  NAMES amivideo libamivideo
  PATHS
    ${PC_AmiVideo_LIBDIR}
    ${PC_AmiVideo_LIBRARIES_DIRS}
  )

mark_as_advanced(AmiVideo_INCLUDE_DIR AmiVideo_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AmiVideo DEFAULT_MSG AmiVideo_INCLUDE_DIR AmiVideo_LIBRARY)
if (AMIVIDEO_FOUND)
  set(AmiVideo_FOUND TRUE)
endif ()

if (AmiVideo_FOUND)
  set(AmiVideo_INCLUDE_DIRS "${AmiVideo_INCLUDE_DIR}")
  set(AmiVideo_LIBRARIES "${AmiVideo_LIBRARY}")
endif()

if (AmiVideo_FOUND AND NOT TARGET AmiVideo::AmiVideo)
  add_library(AmiVideo::AmiVideo UNKNOWN IMPORTED)
  set_target_properties(AmiVideo::AmiVideo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${AmiVideo_INCLUDE_DIR}")
  set_target_properties(AmiVideo::AmiVideo PROPERTIES IMPORTED_LOCATION "${AmiVideo_LIBRARY}")
endif ()
