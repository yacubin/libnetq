#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET JBig2::JBig2)
  set(JBig2_FIND_QUIETLY TRUE)
  set(JBIG2_FOUND TRUE)
  return ()
endif ()

if (JBig2_INCLUDE_DIR AND JBig2_LIBRARY)
  set(JBig2_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_JBig2 JBig2)
endif ()

find_path(JBig2_INCLUDE_DIR
  NAMES jbig2.h
  PATHS
    ${PC_JBig2_INCLUDEDIR}
    ${PC_JBig2_INCLUDE_DIR}
  PATH_SUFFIXES
    jbig2
  )

find_library(JBig2_LIBRARY
  NAMES jbig2 jbig2dec
  PATHS
    ${PC_JBig2_LIBDIR}
    ${PC_JBig2_LIBRARIES_DIRS}
  )

mark_as_advanced(JBig2_INCLUDE_DIR JBig2_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JBig2 REQUIRED_VARS JBig2_LIBRARY JBig2_INCLUDE_DIR)
if (JBIG2_FOUND)
  set(JBig2_FOUND TRUE)
endif ()

if (JBig2_FOUND)
  set(JBig2_INCLUDE_DIRS "${JBig2_INCLUDE_DIR}")
  set(JBig2_LIBRARIES "${JBig2_LIBRARY}")
endif()

if (JBig2_FOUND AND NOT TARGET JBig2::JBig2)
  add_library(JBig2::JBig2 UNKNOWN IMPORTED)
  set_target_properties(JBig2::JBig2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${JBig2_INCLUDE_DIR}")
  set_target_properties(JBig2::JBig2 PROPERTIES IMPORTED_LOCATION "${JBig2_LIBRARY}")
endif ()
