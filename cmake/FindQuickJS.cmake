#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET QuickJS::QuickJS)
  set(QuickJS_FIND_QUIETLY TRUE)
  set(QuickJS_FOUND TRUE)
  return ()
endif ()

if (QuickJS_LIBRARIES AND QuickJS_INCLUDE_DIRS)
  set(QuickJS_FOUND TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_QuickJS QuickJS)
  set(QuickJS_DEFINITIONS ${PC_QuickJS_CFLAGS_OTHER})
endif ()

find_path(QuickJS_INCLUDE_DIR
  NAMES
    quickjs.h
  PATHS
    ${PC_QuickJS_INCLUDEDIR}
    ${PC_QuickJS_INCLUDE_DIRS}
  PATH_SUFFIXES
    quickjs
  )
  
find_library(QuickJS_LIBRARY
  NAMES
    quickjs
  PATHS
    ${PC_QuickJS_LIBDIR}
    ${PC_QuickJS_LIBRARY_DIRS}
  )
  
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(QuickJS DEFAULT_MSG QuickJS_INCLUDE_DIR QuickJS_LIBRARY)

mark_as_advanced(QuickJS_INCLUDE_DIR QuickJS_LIBRARY)
  
if(QUICKJS_FOUND)
  set(QuickJS_FOUND TRUE)
endif ()

if(QuickJS_FOUND)
  set(QuickJS_INCLUDE_DIRS "${QuickJS_INCLUDE_DIR}")
  set(QuickJS_LIBRARIES "${QuickJS_LIBRARY}")
  if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND QuickJS_LIBRARIES m)
  endif ()
endif()

if (QuickJS_FOUND AND NOT TARGET QuickJS::QuickJS)
  add_library(QuickJS::QuickJS INTERFACE IMPORTED)
  set_property(TARGET QuickJS::QuickJS PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${QuickJS_INCLUDE_DIRS})
  set_property(TARGET QuickJS::QuickJS PROPERTY INTERFACE_LINK_LIBRARIES ${QuickJS_LIBRARIES})
endif ()
