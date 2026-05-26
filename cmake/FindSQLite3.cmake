#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET SQLite3::SQLite3)
  set(SQLite3_FIND_QUIETLY TRUE)
  set(SQLite3_FOUND TRUE)
  return ()
endif ()

if (SQLite3_INCLUDE_DIR AND SQLite3_LIBRARY)
  set(SQLite3_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_SQLite3 sqlite3)
  set(SQLite3_DEFINITIONS ${PC_SQLite3_CFLAGS_OTHER})
endif ()

find_path(SQLite3_INCLUDE_DIR
  NAMES
    sqlite3.h
  PATHS
    ${PC_SQLite3_INCLUDEDIR}
    ${PC_SQLite3_INCLUDE_DIRS}
  )

find_library(SQLite3_LIBRARY
  NAMES
    sqlite3
  PATHS
    ${PC_SQLite3_LIBDIR}
    ${PC_SQLite3_LIBRARY_DIRS}
  )

mark_as_advanced(SQLite3_INCLUDE_DIR SQLite3_LIBRARY)

if (SQLite3_FIND_VERSION)
  file(READ sqlite3.h SQLite3_HEADER_CONTENT)
  string(REGEX MATCH "#define +SQLITE_VERSION +\"(([0-9]+)\\.([0-9]+)\\.([0-9]+))\"" _dummy "${SQLite3_HEADER_CONTENT}")
  set(SQLite3_VERSION       "${CMAKE_MATCH_1}")
  set(SQLite3_VERSION_MAJOR "${CMAKE_MATCH_2}")
  set(SQLite3_VERSION_MINOR "${CMAKE_MATCH_3}")
  set(SQLite3_VERSION_PATCH "${CMAKE_MATCH_4}")
  unset(SQLite3_HEADER_CONTENT)
  if ("${SQLite3_FIND_VERSION}" VERSION_GREATER "${SQLite3_VERSION}")
    message(FATAL_ERROR "Required version (${SQLite3_FIND_VERSION}) is higher than found version (${SQLite3_VERSION})")
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SQLite3 DEFAULT_MSG SQLite3_INCLUDE_DIR SQLite3_LIBRARY)
if (SQLITE3_FOUND)
  set(SQLite3_FOUND TRUE)
endif ()

if (SQLite3_FOUND)
  set(SQLite3_INCLUDE_DIRS" ${SQLite3_INCLUDE_DIR}")
  set(SQLite3_LIBRARIES "${SQLite3_LIBRARY}")
endif ()

if (SQLite3_FOUND AND NOT TARGET SQLite3::SQLite3)
  add_library(SQLite3::SQLite3 UNKNOWN IMPORTED)
  set_target_properties(SQLite3::SQLite3 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}")
  set_target_properties(SQLite3::SQLite3 PROPERTIES IMPORTED_LOCATION "${SQLite3_LIBRARY}")
  set_target_properties(SQLite3::SQLite3 PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${SQLite3_DEFINITIONS}")
endif ()
