#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET IFF::IFF)
  set(IFF_FIND_QUIETLY TRUE)
  set(IFF_FOUND TRUE)
  return ()
endif ()

if (IFF_INCLUDE_DIR AND IFF_LIBRARY)
  set(IFF_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_IFF libiff)
endif ()

find_path(IFF_INCLUDE_DIR
  NAMES libiff/iff.h
  PATHS
    ${PC_IFF_INCLUDEDIR}
    ${PC_IFF_INCLUDE_DIR}
  )

find_library(IFF_LIBRARY
  NAMES iff libiff
  PATHS
    ${PC_IFF_LIBDIR}
    ${PC_IFF_LIBRARIES_DIRS}
  )

mark_as_advanced(IFF_INCLUDE_DIR IFF_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(IFF DEFAULT_MSG IFF_INCLUDE_DIR IFF_LIBRARY)

if (IFF_FOUND)
  set(IFF_INCLUDE_DIRS "${IFF_INCLUDE_DIR}")
  set(IFF_LIBRARIES "${IFF_LIBRARY}")
endif()

if (IFF_FOUND AND NOT TARGET IFF::IFF)
  add_library(IFF::IFF UNKNOWN IMPORTED)
  set_target_properties(IFF::IFF PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${IFF_INCLUDE_DIR}")
  set_target_properties(IFF::IFF PROPERTIES IMPORTED_LOCATION "${IFF_LIBRARY}")
endif ()
