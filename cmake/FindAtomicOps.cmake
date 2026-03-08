#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# AtomicOps_FOUND - True if libatomic_ops was found
# AtomicOps_INCLUDE_DIRS - Directories containing libatomic_ops headers
# AtomicOps_LIBRARIES - Libraries to link against libatomic_ops
# AtomicOps_DEFINITIONS - Required compiler definitions for libatomic_ops
#

if (TARGET AtomicOps::AtomicOps)
  set(AtomicOps_FIND_QUIETLY TRUE)
  set(AtomicOps_FOUND TRUE)
  return ()
endif ()

if (AtomicOps_INCLUDE_DIR AND AtomicOps_LIBRARY)
  set(AtomicOps_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_AtomicOps AtomicOps)
  set(AtomicOps_DEFINITIONS ${PC_AtomicOps_CFLAGS_OTHER})
endif ()

find_path(AtomicOps_INCLUDE_DIR
  NAMES atomic_ops.h
  PATHS
    ${PC_AtomicOps_INCLUDEDIR}
    ${PC_AtomicOps_INCLUDE_DIR}
  )

find_library(AtomicOps_LIBRARY
  NAMES atomic_ops
  PATHS
    ${PC_AtomicOps_LIBDIR}
    ${PC_AtomicOps_LIBRARY_DIRS}
  )

mark_as_advanced(AtomicOps_INCLUDE_DIR AtomicOps_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(AtomicOps DEFAULT_MSG AtomicOps_INCLUDE_DIR AtomicOps_LIBRARY)
if (ATOMICOPS_FOUND)
  set(AtomicOps_FOUND TRUE)
endif ()

if (AtomicOps_FOUND)
  set(AtomicOps_LIBRARIES ${AtomicOps_LIBRARY})
  set(AtomicOps_INCLUDE_DIRS ${AtomicOps_INCLUDE_DIR})
endif ()

if (AtomicOps_FOUND AND NOT TARGET AtomicOps::AtomicOps)
  add_library(AtomicOps::AtomicOps UNKNOWN IMPORTED)
  set_target_properties(AtomicOps::AtomicOps PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${AtomicOps_DEFINITIONS}")
  set_target_properties(AtomicOps::AtomicOps PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${AtomicOps_INCLUDE_DIR}")
  set_target_properties(AtomicOps::AtomicOps PROPERTIES IMPORTED_LOCATION "${AtomicOps_LIBRARY}")
endif ()
