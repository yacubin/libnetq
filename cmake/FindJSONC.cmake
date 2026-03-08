#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET JSONC::JSONC)
  set(JSONC_FIND_QUIETLY TRUE)
  set(JSONC_FOUND TRUE)
  return ()
endif ()

if (JSONC_INCLUDE_DIR AND JSONC_LIBRARY)
  set(JSONC_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_JSONC json-c)
  set(JSONC_DEFINITIONS ${PC_JSONC_CFLAGS_OTHER})
endif ()

find_path(JSONC_INCLUDE_DIR
  NAMES json-c/json.h
  PATHS
    ${PC_JSONC_INCLUDEDIR}
    ${PC_JSONC_INCLUDE_DIR}
  )

find_library(JSONC_LIBRARY
  NAMES
    json-c
  PATHS
    ${PC_JSONC_LIBDIR}
    ${PC_JSONC_LIBRARIES_DIRS}
  )

mark_as_advanced(JSONC_INCLUDE_DIR JSONC_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JSONC DEFAULT_MSG JSONC_INCLUDE_DIR JSONC_LIBRARY)

if (JSONC_FOUND)
  set(JSONC_INCLUDE_DIRS ${JSONC_INCLUDE_DIR})
  set(JSONC_LIBRARIES ${JSONC_LIBRARY})
endif ()

if (JSONC_FOUND AND NOT TARGET JSONC::JSONC)
  add_library(JSONC::JSONC UNKNOWN IMPORTED)
  set_target_properties(JSONC::JSONC PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${JSONC_INCLUDE_DIR}")
  set_target_properties(JSONC::JSONC PROPERTIES IMPORTED_LOCATION "${JSONC_LIBRARY}")
endif ()
