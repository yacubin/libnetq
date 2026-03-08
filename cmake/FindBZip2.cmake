#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# BZip2_FOUND - True if libbz2 was found
# BZip2_INCLUDE_DIRS - Directories containing libbz2 headers
# BZip2_LIBRARIES - Libraries to link against libbz2
# BZip2_DEFINITIONS - Required compiler definitions for libbz2
#

if (TARGET BZip2::BZip2)
  set(BZip2_FIND_QUIETLY TRUE)
  set(BZip2_FOUND TRUE)
  return ()
endif ()

if (BZip2_INCLUDE_DIR AND BZip2_LIBRARY)
  set(BZip2_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_BZip2 BZip2)
  set(BZip2_DEFINITIONS ${PC_BZip2_CFLAGS_OTHER})
endif ()

find_path(BZip2_INCLUDE_DIR
  NAMES bzlib.h
  PATHS
    ${PC_BZip2_INCLUDEDIR}
    ${PC_BZip2_INCLUDE_DIR}
  )

find_library(BZip2_LIBRARY
  NAMES bz2 bzip2
  PATHS
    ${PC_BZip2_LIBDIR}
    ${PC_BZip2_LIBRARY_DIRS}
  )

mark_as_advanced(BZip2_INCLUDE_DIR BZip2_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BZip2 DEFAULT_MSG BZip2_INCLUDE_DIR BZip2_LIBRARY)
if (BZIP2_FOUND)
  set(BZip2_FOUND TRUE)
endif ()

if (BZip2_FOUND)
  set(BZip2_LIBRARIES ${BZip2_LIBRARY})
  set(BZip2_INCLUDE_DIRS ${BZip2_INCLUDE_DIR})
endif ()

if (BZip2_FOUND AND NOT TARGET BZip2::BZip2)
  add_library(BZip2::BZip2 UNKNOWN IMPORTED)
  set_target_properties(BZip2::BZip2 PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${BZip2_DEFINITIONS}")
  set_target_properties(BZip2::BZip2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BZip2_INCLUDE_DIR}")
  set_target_properties(BZip2::BZip2 PROPERTIES IMPORTED_LOCATION "${BZip2_LIBRARY}")
endif ()
