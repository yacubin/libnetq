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
  set(CJSON_FOUND TRUE)
  return ()
endif ()

if (CJSON_INCLUDE_DIR AND CJSON_LIBRARY)
  set(CJSON_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
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

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CJSON DEFAULT_MSG CJSON_INCLUDE_DIR CJSON_LIBRARY)

mark_as_advanced(CJSON_INCLUDE_DIR CJSON_LIBRARY)

if(CJSON_FOUND)
  set(CJSON_INCLUDE_DIRS ${CJSON_INCLUDE_DIR})
  set(CJSON_LIBRARIES ${CJSON_LIBRARY})
endif()

if (CJSON_FOUND AND NOT TARGET CJSON::CJSON)
  add_library(CJSON::CJSON UNKNOWN IMPORTED)
  set_target_properties(CJSON::CJSON PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CJSON_INCLUDE_DIR}")
  set_target_properties(CJSON::CJSON PROPERTIES IMPORTED_LOCATION "${CJSON_LIBRARY}")
endif ()
