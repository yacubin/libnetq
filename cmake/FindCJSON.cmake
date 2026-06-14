#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET CJSON::CJSON)
  set(CJSON_FIND_QUIETLY TRUE)
  set(CJSON_FOUND TRUE)
  return ()
endif ()

if (CJSON_INCLUDE_DIR AND CJSON_LIBRARY)
  set(CJSON_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_CJSON libcjson)
  set(CJSON_DEFINITIONS ${PC_CJSON_CFLAGS_OTHER})
endif ()

find_path(CJSON_INCLUDE_DIR
  NAMES cjson/cJSON.h
  PATHS
    ${PC_CJSON_INCLUDEDIR}
    ${PC_CJSON_INCLUDE_DIR}
  )

find_library(CJSON_LIBRARY
  NAMES
    cjson
  PATHS
    ${PC_CJSON_LIBDIR}
    ${PC_CJSON_LIBRARIES_DIRS}
  )

mark_as_advanced(CJSON_INCLUDE_DIR CJSON_LIBRARY)

if (CJSON_INCLUDE_DIR)
  file(READ "${CJSON_INCLUDE_DIR}/cjson/cJSON.h" CJSON_VERSION_CONTENT)

  string(REGEX MATCH "#define +CJSON_VERSION_MAJOR +([0-9]+)" _dummy "${CJSON_VERSION_MAJOR}")
  set(CJSON_VERSION_MAJOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +CJSON_VERSION_MINOR +([0-9]+)" _dummy "${CJSON_VERSION_MINOR}")
  set(CJSON_VERSION_MINOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +CJSON_VERSION_PATCH +([0-9]+)" _dummy "${CJSON_VERSION_PATCH}")
  set(CJSON_VERSION_PATCH "${CMAKE_MATCH_1}")

  set(CJSON_VERSION "${CJSON_VERSION_MAJOR}.${CJSON_VERSION_MINOR}.${CJSON_VERSION_PATCH}")
  unset(CJSON_VERSION_CONTENT)
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CJSON REQUIRED_VARS CJSON_LIBRARY CJSON_INCLUDE_DIR VERSION_VAR CJSON_VERSION)

if (CJSON_FOUND)
  set(CJSON_INCLUDE_DIRS "${CJSON_INCLUDE_DIR}")
  set(CJSON_LIBRARIES "${CJSON_LIBRARY}")
endif ()

if (CJSON_FOUND AND NOT TARGET CJSON::CJSON)
  add_library(CJSON::CJSON UNKNOWN IMPORTED)
  set_target_properties(CJSON::CJSON PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CJSON_INCLUDE_DIR}")
  set_target_properties(CJSON::CJSON PROPERTIES IMPORTED_LOCATION "${CJSON_LIBRARY}")
endif ()
