#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# LibXml2_FOUND - True if LibXml2 was found
# LibXml2_INCLUDE_DIRS - Directories containing LibXml2 headers
# LibXml2_LIBRARIES - Libraries to link against LibXml2
# LibXml2_DEFINITIONS - Required compiler definitions for LibXml2
#

if (TARGET LibXml2::LibXml2)
  set(LibXml2_FIND_QUIETLY TRUE)
  set(LibXml2_FOUND TRUE)
  return ()
endif ()

if (LibXml2_INCLUDE_DIR AND LibXml2_LIBRARY)
  set(LibXml2_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibXml2 libxml-2.0)
  set(LibXml2_DEFINITIONS ${PC_LibXml2_CFLAGS_OTHER})
endif ()

find_path(LibXml2_INCLUDE_DIR
  NAMES libxml/xpath.h
  PATHS
    ${PC_LibXml2_INCLUDEDIR}
    ${PC_LibXml2_INCLUDE_DIRS}
  PATH_SUFFIXES
    libxml2
  )
  
find_library(LibXml2_LIBRARY
  NAMES libxml2
  PATHS
    ${PC_LibXml2_LIBDIR}
    ${PC_LibXml2_LIBRARY_DIRS}
  )

mark_as_advanced(LibXml2_INCLUDE_DIR LibXml2_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibXml2 DEFAULT_MSG LibXml2_INCLUDE_DIR LibXml2_LIBRARY)
if (LIBXML2_FOUND)
  set(LibXml2_FOUND TRUE)
endif ()

if (LibXml2_FOUND)
  set(LibXml2_INCLUDE_DIRS ${LibXml2_INCLUDE_DIR})
  set(LibXml2_LIBRARIES ${LibXml2_LIBRARY})
endif ()

if (LibXml2_FOUND AND NOT TARGET LibXml2::LibXml2)
  add_library(LibXml2::LibXml2 UNKNOWN IMPORTED)
  set_target_properties(LibXml2::LibXml2 PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${LibXml2_DEFINITIONS}")
  set_target_properties(LibXml2::LibXml2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibXml2_INCLUDE_DIR}")
  set_target_properties(LibXml2::LibXml2 PROPERTIES IMPORTED_LOCATION "${LibXml2_LIBRARY}")
endif ()
