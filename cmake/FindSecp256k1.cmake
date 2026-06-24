#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Bitcoin::Secp256k1)
  set(Secp256k1_FIND_QUIETLY TRUE)
  set(Secp256k1_FOUND TRUE)
  return ()
endif ()

if (Secp256k1_LIBRARIES AND Secp256k1_INCLUDE_DIRS)
  set(Secp256k1_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_Secp256k1 libsecp256k1)
  set(Secp256k1_DEFINITIONS ${PC_Secp256k1_CFLAGS_OTHER})
endif ()

find_path(Secp256k1_INCLUDE_DIR
  NAMES
    secp256k1.h
  PATHS
    ${PC_Secp256k1_INCLUDEDIR}
    ${PC_Secp256k1_INCLUDE_DIRS}
  )

find_library(Secp256k1_LIBRARY
  NAMES
    secp256k1
  PATHS
    ${PC_Secp256k1_LIBDIR}
    ${PC_Secp256k1_LIBRARY_DIRS}
  )

get_filename_component(Secp256k1_LIBRARY_EXTNAME "${Secp256k1_LIBRARY}" LAST_EXT)
if ("${Secp256k1_LIBRARY_EXTNAME}" MATCHES "(.a|.lib)$")
  list(APPEND Secp256k1_DEFINITIONS SECP256K1_STATIC)
endif ()

mark_as_advanced(Secp256k1_INCLUDE_DIR Secp256k1_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Secp256k1 REQUIRED_VARS Secp256k1_LIBRARY Secp256k1_INCLUDE_DIR)
if (SECP256K1_FOUND)
  set(Secp256k1_FOUND TRUE)
endif ()

if (Secp256k1_FOUND)
  set(Secp256k1_INCLUDE_DIRS "${Secp256k1_INCLUDE_DIR}")
  set(Secp256k1_LIBRARIES "${Secp256k1_LIBRARY}")
endif ()

if (Secp256k1_FOUND AND NOT TARGET Bitcoin::Secp256k1)
  add_library(Bitcoin::Secp256k1 UNKNOWN IMPORTED)
  set_target_properties(Bitcoin::Secp256k1 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Secp256k1_INCLUDE_DIR}")
  set_target_properties(Bitcoin::Secp256k1 PROPERTIES IMPORTED_LOCATION "${Secp256k1_LIBRARY}")
  set_target_properties(Bitcoin::Secp256k1 PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Secp256k1_DEFINITIONS}")
endif ()
