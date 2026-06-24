#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# CJOSE_FOUND - True if libcjose was found
# CJOSE_INCLUDE_DIRS - Directories containing libcjose headers
# CJOSE_LIBRARIES - Libraries to link against libcjose
# CJOSE_DEFINITIONS - Required compiler definitions for libcjose
#

if (TARGET CJOSE::CJOSE)
  set(CJOSE_FIND_QUIETLY TRUE)
  set(CJOSE_FOUND TRUE)
  return ()
endif ()

if (CJOSE_INCLUDE_DIR AND CJOSE_LIBRARY)
  set(CJOSE_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_CJOSE cjose)
  set(CJOSE_DEFINITIONS ${PC_CJOSE_CFLAGS_OTHER})
endif ()

find_path(CJOSE_INCLUDE_DIR
  NAMES cjose/cjose.h
  PATHS
    ${PC_CJOSE_INCLUDEDIR}
    ${PC_CJOSE_INCLUDE_DIRS}
  )

find_library(CJOSE_LIBRARY
  NAMES cjose
  PATHS
    ${PC_CJOSE_LIBDIR}
    ${PC_CJOSE_LIBRARY_DIRS}
  )

mark_as_advanced(CJOSE_INCLUDE_DIR CJOSE_LIBRARY)

if (CJOSE_INCLUDE_DIR)
  file(READ "${CJOSE_INCLUDE_DIR}/cjose/version.h" CJOSE_VERSION_CONTENT)
  string(REGEX MATCH "#define +CJOSE_VERSION +\"(([0-9]+)\\.([0-9]+)\\.([0-9]+))\"" _dummy "${CJOSE_VERSION_CONTENT}")
  set(CJOSE_VERSION       "${CMAKE_MATCH_1}")
  set(CJOSE_VERSION_MAJOR "${CMAKE_MATCH_2}")
  set(CJOSE_VERSION_MINOR "${CMAKE_MATCH_3}")
  set(CJOSE_VERSION_PATCH "${CMAKE_MATCH_4}")
  unset(CJOSE_VERSION_CONTENT)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CJOSE REQUIRED_VARS CJOSE_LIBRARY CJOSE_INCLUDE_DIR VERSION_VAR CJOSE_VERSION)

if (CJOSE_FOUND)
  set(CJOSE_INCLUDE_DIRS ${CJOSE_INCLUDE_DIR})
  set(CJOSE_LIBRARIES ${CJOSE_LIBRARY})
endif ()

if (CJOSE_FOUND AND NOT TARGET CJOSE::CJOSE)
  add_library(CJOSE::CJOSE UNKNOWN IMPORTED)
  set_target_properties(CJOSE::CJOSE PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CJOSE_INCLUDE_DIR}")
  set_target_properties(CJOSE::CJOSE PROPERTIES IMPORTED_LOCATION "${CJOSE_LIBRARY}")
  set_target_properties(CJOSE::CJOSE PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${CJOSE_DEFINITIONS}")
endif ()
