#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET TIFF::TIFF)
  set(TIFF_FIND_QUIETLY TRUE)
  set(TIFF_FOUND TRUE)
  return ()
endif ()

if (TIFF_INCLUDE_DIR AND TIFF_LIBRARY)
  set(TIFF_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_TIFF libTIFF)
endif ()

find_path(TIFF_INCLUDE_DIR
  NAMES tiff.h
  PATHS
    ${PC_TIFF_INCLUDEDIR}
    ${PC_TIFF_INCLUDE_DIR}
  )

find_library(TIFF_LIBRARY
  NAMES tiff
  PATHS
    ${PC_TIFF_LIBDIR}
    ${PC_TIFF_LIBRARIES_DIRS}
  )

mark_as_advanced(TIFF_INCLUDE_DIR TIFF_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TIFF DEFAULT_MSG TIFF_INCLUDE_DIR TIFF_LIBRARY)

if (TIFF_FOUND)
  set(TIFF_INCLUDE_DIRS "${TIFF_INCLUDE_DIR}")
  set(TIFF_LIBRARIES "${TIFF_LIBRARY}")
endif()

if (TIFF_FOUND AND NOT TARGET TIFF::TIFF)
  add_library(TIFF::TIFF UNKNOWN IMPORTED)
  set_target_properties(TIFF::TIFF PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${TIFF_INCLUDE_DIR}")
  set_target_properties(TIFF::TIFF PROPERTIES IMPORTED_LOCATION "${TIFF_LIBRARY}")
endif ()
