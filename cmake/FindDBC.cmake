#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET DBC::DBC)
  set(DBC_FIND_QUIETLY TRUE)
  set(DBC_FOUND TRUE)
  return ()
endif ()

if (DBC_INCLUDE_DIR AND DBC_LIBRARY)
  set(DBC_FIND_QUIETLY TRUE)
endif ()

find_path(DBC_INCLUDE_DIR
  NAMES dbc/DBCTypes.h
  )

find_library(DBC_LIBRARY
  NAMES dbc
  )

mark_as_advanced(DBC_INCLUDE_DIR DBC_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DBC REQUIRED_VARS DBC_LIBRARY DBC_INCLUDE_DIR)

if (DBC_FOUND)
  set(DBC_INCLUDE_DIRS "${DBC_INCLUDE_DIR}")
  set(DBC_LIBRARIES "${DBC_LIBRARY}")
endif()

if (DBC_FOUND AND NOT TARGET DBC::DBC)
  add_library(DBC::DBC UNKNOWN IMPORTED)
  set_target_properties(DBC::DBC PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${DBC_INCLUDE_DIR}")
  set_target_properties(DBC::DBC PROPERTIES IMPORTED_LOCATION "${DBC_LIBRARY}")
endif ()
