#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LibNetQ::LibNetQ)
    set(LibNetQ_FIND_QUIETLY TRUE)
    set(LibNetQ_FOUND TRUE)
    set(LibNetQ_FOUND TRUE)
  return ()
endif ()

if (LibNetQ_INCLUDE_DIR AND LibNetQ_LIBRARY)
  set(LibNetQ_FIND_QUIETLY TRUE)
endif ()

if (NOT WIN32)
  find_package(PkgConfig)
  pkg_check_modules(PC_Netq Netq)
  set(LibNetQ_DEFINITIONS ${PC_LibNetQ_CFLAGS_OTHER})
endif (NOT WIN32)

find_path(LibNetQ_INCLUDE_DIR
  NAMES
    libnetq/Assert.h
  PATHS
    ${PC_LibNetQ_INCLUDEDIR}
    ${PC_LibNetQ_INCLUDE_DIR}
  )

if (LibNetQ_INCLUDE_DIR)
  set(LibNetQ_INCLUDE_DIRS ${LibNetQ_INCLUDE_DIR})
endif ()

find_library(LibNetQ_LIBRARY
  NAMES
    netq
  PATHS
    ${PC_LibNetQ_LIBDIR}
    ${PC_LibNetQ_LIBRARIES_DIRS}
  )

set(LibNetQ_DEFINITIONS
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibNetQ DEFAULT_MSG LibNetQ_INCLUDE_DIR LibNetQ_LIBRARY)

mark_as_advanced(LibNetQ_INCLUDE_DIR LibNetQ_LIBRARY)

if (LIBNETQ_FOUND OR LibNetQ_FOUND)
  set(LibNetQ_FOUND 1)
  set(LibNetQ_INCLUDE_DIRS "${LibNetQ_INCLUDE_DIR}")
  set(LibNetQ_LIBRARIES "${LibNetQ_LIBRARY}")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  list(APPEND LibNetQ_LIBRARIES ws2_32 wsock32)
endif ()

if (LibNetQ_FOUND AND NOT TARGET LibNetQ::LibNetQ)
  add_library(LibNetQ::LibNetQ UNKNOWN IMPORTED)
  set_target_properties(LibNetQ::LibNetQ PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${LibNetQ_DEFINITIONS}")
  set_target_properties(LibNetQ::LibNetQ PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibNetQ_INCLUDE_DIRS}")
  set_target_properties(LibNetQ::LibNetQ PROPERTIES IMPORTED_LOCATION "${LibNetQ_LIBRARY}")
  set_target_properties(LibNetQ::LibNetQ PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES "C")
  if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(LibNetQ::LibNetQ PROPERTIES INTERFACE_LINK_LIBRARIES ws2_32 wsock32)
  endif ()
endif ()
