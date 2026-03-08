#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# Apr_FOUND - True if libexpat was found
# Apr_INCLUDE_DIRS - Directories containing libexpat headers
# Apr_LIBRARIES - Libraries to link against libexpat
# Apr_DEFINITIONS - Required compiler definitions for libexpat
#

if (TARGET Apache::Apr)
  set(Apr_FIND_QUIETLY TRUE)
  set(Apr_FOUND TRUE)
  return ()
endif ()

if (Apr_INCLUDE_DIR AND Apr_LIBRARY)
  set(Apr_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_Apr Apr)
  set(Apr_DEFINITIONS ${PC_Apr_CFLAGS_OTHER})
endif ()

if (Apr_FIND_VERSION)
  string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" Apr_FIND_MAJOR_VERSION "${Apr_FIND_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" Apr_FIND_MINOR_VERSION "${Apr_FIND_VERSION}")
  string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" Apr_FIND_PATCH_VERSION "${Apr_FIND_VERSION}")
else ()
  set(Apr_FIND_MAJOR_VERSION 1)
  set(Apr_FIND_MINOR_VERSION 0)
  set(Apr_FIND_PATCH_VERSION 0)
endif ()

find_path(Apr_INCLUDE_DIR
  NAMES apr.h
  PATHS
    ${PC_Apr_INCLUDEDIR}
    ${PC_Apr_INCLUDE_DIRS}
  PATH_SUFFIXES
    apr-${Apr_FIND_MAJOR_VERSION}
  )

find_library(Apr_LIBRARY
  NAMES
    apr-${Apr_FIND_MAJOR_VERSION}
  PATHS
    ${PC_Apr_LIBDIR}
    ${PC_Apr_LIBRARY_DIRS}
  )

mark_as_advanced(Apr_INCLUDE_DIR Apr_LIBRARY)

if (Apr_FIND_VERSION AND Apr_INCLUDE_DIR)
  if (EXISTS "${Apr_INCLUDE_DIR}/apr_version.h")
    file(READ "${Apr_INCLUDE_DIR}/apr_version.h" Apr_VERSION_CONTENT)

    string(REGEX MATCH "#define +APR_MAJOR_VERSION +([0-9]+)" _dummy "${Apr_VERSION_CONTENT}")
    set(Apr_MAJOR_VERSION "${CMAKE_MATCH_1}")

    string(REGEX MATCH "#define +APR_MINOR_VERSION +([0-9]+)" _dummy "${Apr_VERSION_CONTENT}")
    set(Apr_MINOR_VERSION "${CMAKE_MATCH_1}")

    string(REGEX MATCH "#define +APR_PATCH_VERSION +([0-9]+)" _dummy "${Apr_VERSION_CONTENT}")
    set(Apr_PATCH_VERSION "${CMAKE_MATCH_1}")

    set(Apr_VERSION "${Apr_MAJOR_VERSION}.${Apr_MINOR_VERSION}.${Apr_PATCH_VERSION}")
    unset(Apr_VERSION_CONTENT)
  endif ()

  if ("${Apr_FIND_VERSION}" VERSION_GREATER "${Apr_VERSION}")
    message(FATAL_ERROR "Required version (${Apr_FIND_VERSION}) is higher than found version (${Apr_VERSION})")
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Apr DEFAULT_MSG Apr_INCLUDE_DIR Apr_LIBRARY)
if (APR_FOUND)
  set(Apr_FOUND TRUE)
endif ()

if (Apr_FOUND)
  set(Apr_INCLUDE_DIRS ${Apr_INCLUDE_DIR})
  set(Apr_LIBRARIES ${Apr_LIBRARY})
endif ()

if (Apr_FOUND AND NOT TARGET Apache::Apr)
  add_library(Apache::Apr UNKNOWN IMPORTED)
  set_target_properties(Apache::Apr PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Apr_DEFINITIONS}")
  set_target_properties(Apache::Apr PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Apr_INCLUDE_DIR}")
  set_target_properties(Apache::Apr PROPERTIES IMPORTED_LOCATION "${Apr_LIBRARY}")
endif ()
