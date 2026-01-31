#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET JBig2Enc::JBig2Enc)
  set(JBig2Enc_FIND_QUIETLY TRUE)
  set(JBig2Enc_FOUND TRUE)
  return ()
endif ()

if (JBig2Enc_INCLUDE_DIR AND JBig2Enc_LIBRARY)
  set(JBig2Enc_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_JBig2Enc JBig2Enc)
endif ()

find_path(JBig2Enc_INCLUDE_DIR
  NAMES jbig2enc.h
  PATHS
    ${PC_JBig2Enc_INCLUDEDIR}
    ${PC_JBig2Enc_INCLUDE_DIR}
  )

find_library(JBig2Enc_LIBRARY
  NAMES jbig2enc
  PATHS
    ${PC_JBig_LIBDIR}
    ${PC_JBig_LIBRARIES_DIRS}
  )

mark_as_advanced(JBig2Enc_INCLUDE_DIR JBig2Enc_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(JBig2Enc DEFAULT_MSG JBig2Enc_INCLUDE_DIR JBig2Enc_LIBRARY)
if (JBIG2ENC_FOUND)
  set(JBig2Enc_FOUND TRUE)
endif ()

if (JBig2Enc_FOUND)
  set(JBig2Enc_INCLUDE_DIRS ${JBig2Enc_INCLUDE_DIR})
  set(JBig2Enc_LIBRARIES ${JBig2Enc_LIBRARY})
endif()

if (JBig2Enc_FOUND AND NOT TARGET JBig2Enc::JBig2Enc)
  add_library(JBig2Enc::JBig2Enc UNKNOWN IMPORTED)
  set_target_properties(JBig2Enc::JBig2Enc PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${JBig2Enc_INCLUDE_DIR}")
  set_target_properties(JBig2Enc::JBig2Enc PROPERTIES IMPORTED_LOCATION "${JBig2Enc_LIBRARY}")
endif ()
