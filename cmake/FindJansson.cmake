#
# MIT License
#
# Copyright (c) 2022-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Jansson::Jansson)
  set(Jansson_FIND_QUIETLY TRUE)
  set(Jansson_FOUND TRUE)
  set(JANSSON_FOUND TRUE)
  return ()
endif ()

if (Jansson_INCLUDE_DIR AND Jansson_LIBRARY)
  set(Jansson_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_JANSSON Jansson)
  set(Jansson_DEFINITIONS ${PC_JANSSON_CFLAGS_OTHER})
endif ()

find_path(Jansson_INCLUDE_DIR
  NAMES jansson_config.h jansson.h
  PATHS
    ${PC_JANSSON_INCLUDEDIR}
    ${PC_JANSSON_INCLUDE_DIR}
  PATH_SUFFIXES
    jansson
  )

find_library(Jansson_LIBRARY
  NAMES
    jansson
  PATHS
    ${PC_JANSSON_LIBDIR}
    ${PC_JANSSON_LIBRARIES_DIRS}
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Jansson DEFAULT_MSG Jansson_INCLUDE_DIR Jansson_LIBRARY)

mark_as_advanced(Jansson_INCLUDE_DIR Jansson_LIBRARY)

if(JANSSON_FOUND)
  set(Jansson_FOUND TRUE)
endif()

if(Jansson_FOUND)
  set(Jansson_INCLUDE_DIRS ${Jansson_INCLUDE_DIR})
  set(Jansson_LIBRARIES ${Jansson_LIBRARY})
endif()

if (Jansson_FOUND AND NOT TARGET Jansson::Jansson)
  add_library(Jansson::Jansson INTERFACE IMPORTED)
  set_property(TARGET Jansson::Jansson PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Jansson_INCLUDE_DIR}")
  set_property(TARGET Jansson::Jansson PROPERTY INTERFACE_LINK_LIBRARIES "${Jansson_LIBRARY}")
endif ()
