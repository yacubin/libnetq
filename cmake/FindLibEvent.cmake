#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# LibEvent_FOUND - True if libevent was found
# LibEvent_INCLUDE_DIRS - Directories containing libevent headers
# LibEvent_LIBRARIES - Libraries to link against libevent
# LibEvent_DEFINITIONS - Required compiler definitions for libevent
#

if (TARGET LibEvent::LibEvent)
  set(LibEvent_FIND_QUIETLY TRUE)
  set(LibEvent_FOUND TRUE)
  return ()
endif ()

if (LibEvent_INCLUDE_DIR AND LibEvent_LIBRARY)
  set(LibEvent_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibEvent LibEvent)
  set(LibEvent_DEFINITIONS ${PC_LibEvent_CFLAGS_OTHER})
endif ()

find_path(LibEvent_INCLUDE_DIR
  NAMES event.h
  PATHS
    ${PC_LibEvent_INCLUDEDIR}
    ${PC_LibEvent_INCLUDE_DIR}
  PATH_SUFFIXES
    libevent
  )

find_library(LibEvent_LIBRARY
  NAMES event
  PATHS
    ${PC_LibEvent_LIBDIR}
    ${PC_LibEvent_LIBRARY_DIRS}
  )

mark_as_advanced(LibEvent_INCLUDE_DIR LibEvent_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibEvent DEFAULT_MSG LibEvent_INCLUDE_DIR LibEvent_LIBRARY)
if (LIBEVENT_FOUND)
  set(LibEvent_FOUND TRUE)
endif ()

if (LibEvent_FOUND)
  set(LibEvent_LIBRARIES ${LibEvent_LIBRARY})
  set(LibEvent_INCLUDE_DIRS ${LibEvent_INCLUDE_DIR})
endif ()

if (LibEvent_FOUND AND NOT TARGET LibEvent::LibEvent)
  add_library(LibEvent::LibEvent UNKNOWN IMPORTED)
  set_target_properties(LibEvent::LibEvent PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${LibEvent_DEFINITIONS}")
  set_target_properties(LibEvent::LibEvent PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibEvent_INCLUDE_DIR}")
  set_target_properties(LibEvent::LibEvent PROPERTIES IMPORTED_LOCATION "${LibEvent_LIBRARY}")
endif ()
