#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET CivetWeb::CivetWeb)
  set(CivetWeb_FIND_QUIETLY TRUE)
  set(CivetWeb_FOUND TRUE)
  return ()
endif ()

if (CivetWeb_INCLUDE_DIR AND CivetWeb_LIBRARY)
  set(CivetWeb_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_CivetWeb civetweb)
  set(CivetWeb_DEFINITIONS ${PC_CivetWeb_CFLAGS_OTHER})
endif ()

find_path(CivetWeb_INCLUDE_DIR
  NAMES civetweb.h
  PATHS
    ${PC_CivetWeb_INCLUDEDIR}
    ${PC_CivetWeb_INCLUDE_DIR}
  )

find_library(CivetWeb_LIBRARY
  NAMES
    civetweb
  PATHS
    ${PC_CivetWeb_LIBDIR}
    ${PC_CivetWeb_LIBRARIES_DIRS}
  )

mark_as_advanced(CivetWeb_INCLUDE_DIR CivetWeb_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CivetWeb DEFAULT_MSG CivetWeb_INCLUDE_DIR CivetWeb_LIBRARY)
if (CIVETWEB_FOUND)
  set(CivetWeb_FOUND TRUE)
endif ()

if (CivetWeb_FOUND)
  set(CivetWeb_INCLUDE_DIRS ${CivetWeb_INCLUDE_DIR})
  set(CivetWeb_LIBRARIES ${CivetWeb_LIBRARY})
endif ()

if (CivetWeb_FOUND AND NOT TARGET CivetWeb::CivetWeb)
  add_library(CivetWeb::CivetWeb UNKNOWN IMPORTED)
  set_target_properties(CivetWeb::CivetWeb PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${CivetWeb_INCLUDE_DIR}")
  set_target_properties(CivetWeb::CivetWeb PROPERTIES IMPORTED_LOCATION "${CivetWeb_LIBRARY}")
endif ()
