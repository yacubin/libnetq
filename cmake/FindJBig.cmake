#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET JBig::JBig)
  set(JBig_FIND_QUIETLY TRUE)
  set(JBig_FOUND TRUE)
  return ()
endif ()

if (JBig_INCLUDE_DIR AND JBig_LIBRARY)
  set(JBig_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_JBig JBig)
endif ()

find_path(JBig_INCLUDE_DIR
  NAMES jbig.h
  PATHS
    ${PC_JBig_INCLUDEDIR}
    ${PC_JBig_INCLUDE_DIR}
  PATH_SUFFIXES
    libjbig
  )

find_library(JBig_LIBRARY
  NAMES jbig
  PATHS
    ${PC_JBig_LIBDIR}
    ${PC_JBig_LIBRARIES_DIRS}
  )

mark_as_advanced(JBig_INCLUDE_DIR JBig_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JBig DEFAULT_MSG JBig_INCLUDE_DIR JBig_LIBRARY)
if (JBIG_FOUND)
  set(JBig_FOUND TRUE)
endif ()

if (JBig_FOUND)
  set(JBig_INCLUDE_DIRS ${JBig_INCLUDE_DIR})
  set(JBig_LIBRARIES ${JBig_LIBRARY})
endif()

if (JBig_FOUND AND NOT TARGET JBig::JBig)
  add_library(JBig::JBig UNKNOWN IMPORTED)
  set_target_properties(JBig::JBig PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${JBig_INCLUDE_DIR}")
  set_target_properties(JBig::JBig PROPERTIES IMPORTED_LOCATION "${JBig_LIBRARY}")
endif ()
