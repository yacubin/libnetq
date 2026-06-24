#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# XmlSec1_FOUND - True if XmlSec1 was found
# XmlSec1_INCLUDE_DIRS - Directories containing XmlSec1 headers
# XmlSec1_LIBRARIES - Libraries to link against XmlSec1
# XmlSec1_DEFINITIONS - Required compiler definitions for XmlSec1
#

if (TARGET XmlSec1::XmlSec1)
  set(XmlSec1_FIND_QUIETLY TRUE)
  set(XmlSec1_FOUND TRUE)
  return ()
endif ()

if (XmlSec1_INCLUDE_DIR AND XmlSec1_LIBRARY)
  set(XmlSec1_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_XmlSec1 xmlsec1)
  set(XmlSec1_DEFINITIONS ${PC_XmlSec1_CFLAGS_OTHER})
endif ()

find_path(XmlSec1_INCLUDE_DIR
  NAMES xmlsec/xmlsec.h
  PATHS
    ${PC_XmlSec1_INCLUDEDIR}
    ${PC_XmlSec1_INCLUDE_DIRS}
  PATH_SUFFIXES
    xmlsec1
  )
  
find_library(XmlSec1_LIBRARY
  NAMES xmlsec1
  PATHS
    ${PC_XmlSec1_LIBDIR}
    ${PC_XmlSec1_LIBRARY_DIRS}
  )

mark_as_advanced(XmlSec1_INCLUDE_DIR XmlSec1_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(XmlSec1 REQUIRED_VARS XmlSec1_LIBRARY XmlSec1_INCLUDE_DIR)
if (XMLSEC1_FOUND)
  set(XmlSec1_FOUND TRUE)
endif ()

if (XmlSec1_FOUND)
  set(XmlSec1_INCLUDE_DIRS "${XmlSec1_INCLUDE_DIR}")
  set(XmlSec1_LIBRARIES "${XmlSec1_LIBRARY}")
endif ()

if (XmlSec1_FOUND AND NOT TARGET XmlSec1::XmlSec1)
  add_library(XmlSec1::XmlSec1 UNKNOWN IMPORTED)
  set_target_properties(XmlSec1::XmlSec1 PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${XmlSec1_DEFINITIONS}")
  set_target_properties(XmlSec1::XmlSec1 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${XmlSec1_INCLUDE_DIR}")
  set_target_properties(XmlSec1::XmlSec1 PROPERTIES IMPORTED_LOCATION "${XmlSec1_LIBRARY}")
endif ()
