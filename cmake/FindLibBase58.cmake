#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Bitcoin::LibBase58)
  set(LibBase58_FIND_QUIETLY TRUE)
  set(LibBase58_FOUND TRUE)
  return ()
endif ()

if (LibBase58_LIBRARIES AND LibBase58_INCLUDE_DIRS)
  set(LibBase58_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibBase58 libbase58)
  set(LibBase58_DEFINITIONS ${PC_LibBase58_CFLAGS_OTHER})
endif ()

find_path(LibBase58_INCLUDE_DIR
  NAMES
    libbase58.h
  PATHS
    ${PC_LibBase58_INCLUDEDIR}
    ${PC_LibBase58_INCLUDE_DIRS}
  )

find_library(LibBase58_LIBRARY
  NAMES
    base58
  PATHS
    ${PC_LibBase58_LIBDIR}
    ${PC_LibBase58_LIBRARY_DIRS}
  )

mark_as_advanced(LibBase58_INCLUDE_DIR LibBase58_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibBase58 DEFAULT_MSG LibBase58_INCLUDE_DIR LibBase58_LIBRARY)
if (LIBBASEE58_FOUND)
  set(LibBase58_FOUND TRUE)
endif ()

if (LibBase58_FOUND)
  set(LibBase58_INCLUDE_DIRS ${LibBase58_INCLUDE_DIR})
  set(LibBase58_LIBRARIES ${LibBase58_LIBRARY})
endif ()

if (LibBase58_FOUND AND NOT TARGET Bitcoin::LibBase58)
  add_library(Bitcoin::LibBase58 UNKNOWN IMPORTED)
  set_target_properties(Bitcoin::LibBase58 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibBase58_INCLUDE_DIR}")
  set_target_properties(Bitcoin::LibBase58 PROPERTIES IMPORTED_LOCATION "${LibBase58_LIBRARY}")
endif ()
