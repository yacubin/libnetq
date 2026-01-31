#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET ILBM::ILBM)
  set(ILBM_FIND_QUIETLY TRUE)
  set(ILBM_FOUND TRUE)
  return ()
endif ()

if (ILBM_INCLUDE_DIR AND ILBM_LIBRARY)
  set(ILBM_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_ILBM libilbm)
endif ()

find_path(ILBM_INCLUDE_DIR
  NAMES libilbm/ilbm.h
  PATHS
    ${PC_ILBM_INCLUDEDIR}
    ${PC_ILBM_INCLUDE_DIR}
  )

find_library(ILBM_LIBRARY
  NAMES ilbm libilbm
  PATHS
    ${PC_ILBM_LIBDIR}
    ${PC_ILBM_LIBRARIES_DIRS}
  )

mark_as_advanced(ILBM_INCLUDE_DIR ILBM_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ILBM DEFAULT_MSG ILBM_INCLUDE_DIR ILBM_LIBRARY)

if (ILBM_FOUND)
  set(ILBM_INCLUDE_DIRS "${ILBM_INCLUDE_DIR}")
  set(ILBM_LIBRARIES "${ILBM_LIBRARY}")
endif()

if (ILBM_FOUND AND NOT TARGET ILBM::ILBM)
  add_library(ILBM::ILBM UNKNOWN IMPORTED)
  set_target_properties(ILBM::ILBM PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${ILBM_INCLUDE_DIR}")
  set_target_properties(ILBM::ILBM PROPERTIES IMPORTED_LOCATION "${ILBM_LIBRARY}")
endif ()
