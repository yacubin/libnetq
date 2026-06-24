#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LibJWT::LibJWT)
  set(LibJWT_FIND_QUIETLY TRUE)
  set(LibJWT_FOUND TRUE)
  return ()
endif ()

if (LibJWT_LIBRARIES AND LibJWT_INCLUDE_DIRS)
  set(LibJWT_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibJWT libjwt)
  set(LibJWT_DEFINITIONS ${PC_LibJWT_CFLAGS_OTHER})
endif ()

find_path(LibJWT_INCLUDE_DIR
  NAMES jwt.h
  PATHS
    ${PC_LibJWT_INCLUDEDIR}
    ${PC_LibJWT_INCLUDE_DIRS}
  )

find_library(LibJWT_LIBRARY
  NAMES jwt
  PATHS
    ${PC_LibJWT_LIBDIR}
    ${PC_LibJWT_LIBRARY_DIRS}
  )

mark_as_advanced(LibJWT_INCLUDE_DIR LibJWT_LIBRARY)

if (PC_LibJWT_VERSION)
  set(LibJWT_VERSION "${PC_LibJWT_VERSION}")
elseif (LibJWT_INCLUDE_DIR)
  if (EXISTS "${LibJWT_INCLUDE_DIR}/jwt_export.h")
    file(READ "${LibJWT_INCLUDE_DIR}/jwt_export.h" LibJWT_VERSION_CONTENT)
    string(REGEX MATCH "#define[ \t]+JWT_VERSION_STRING[ \t]+\"(([0-9]+)\\.([0-9]+)\\.([0-9]+))\"" _dummy "${LibJWT_VERSION_CONTENT}")
  elseif (LibJWT_LIBRARY)
    get_filename_component(LibJWT_LIBRARY_DIR "${LibJWT_LIBRARY}" DIRECTORY)
    if (EXISTS "${LibJWT_LIBRARY_DIR}/pkgconfig/libjwt.pc")
      file(READ "${LibJWT_LIBRARY_DIR}/pkgconfig/libjwt.pc" LibJWT_VERSION_CONTENT)
      string(REGEX MATCH "Version:[ \t]+(([0-9]+)\\.([0-9]+)\\.([0-9]+))" _dummy "${LibJWT_VERSION_CONTENT}")
    endif ()
  endif ()
  if (LibJWT_VERSION_CONTENT)
    set(LibJWT_VERSION       "${CMAKE_MATCH_1}")
    set(LibJWT_VERSION_MAJOR "${CMAKE_MATCH_2}")
    set(LibJWT_VERSION_MINOR "${CMAKE_MATCH_3}")
    set(LibJWT_VERSION_PATCH "${CMAKE_MATCH_4}")
    unset(LibJWT_VERSION_CONTENT)
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibJWT REQUIRED_VARS LibJWT_LIBRARY LibJWT_INCLUDE_DIR VERSION_VAR LibJWT_VERSION)
if (LIBJWT_FOUND)
  set(LibJWT_FOUND TRUE)
endif ()

if (LibJWT_FOUND)
  set(LibJWT_INCLUDE_DIRS "${LibJWT_INCLUDE_DIR}")
  set(LibJWT_LIBRARIES "${LibJWT_LIBRARY}")
endif ()

if (LibJWT_FOUND AND NOT TARGET LibJWT::LibJWT)
  add_library(LibJWT::LibJWT UNKNOWN IMPORTED)
  set_target_properties(LibJWT::LibJWT PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibJWT_INCLUDE_DIR}")
  set_target_properties(LibJWT::LibJWT PROPERTIES IMPORTED_LOCATION "${LibJWT_LIBRARY}")
endif ()
