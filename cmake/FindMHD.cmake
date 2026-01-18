#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET MHD::MHD)
    set(MHD_FIND_QUIETLY TRUE)
    set(MHD_FOUND TRUE)
  return ()
endif ()

if (MHD_INCLUDE_DIR AND MHD_LIBRARY)
  set(MHD_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_MHD libmicrohttpd)
  set(MHD_DEFINITIONS ${PC_MHD_CFLAGS_OTHER})
endif ()

find_path(MHD_INCLUDE_DIR
  NAMES microhttpd.h
  PATHS
    ${PC_MHD_INCLUDEDIR}
    ${PC_MHD_INCLUDE_DIR}
  PATH_SUFFIXES
    libmicrohttpd
  DOC "microhttpd include dir"
  )

find_library(MHD_LIBRARY
  NAMES microhttpd microhttpd-10 libmicrohttpd libmicrohttpd-dll
  PATHS
    ${PC_MHD_LIBDIR}
    ${PC_MHD_LIBRARY_DIRS}
  DOC "microhttpd library"
  )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MHD DEFAULT_MSG MHD_LIBRARY MHD_INCLUDE_DIR)

mark_as_advanced(MHD_INCLUDE_DIR MHD_LIBRARY)

if(MHD_FOUND)
  set(MHD_INCLUDE_DIRS ${MHD_INCLUDE_DIR})
  set(MHD_LIBRARIES ${MHD_LIBRARY})
endif()

if (MHD_FOUND AND NOT TARGET MHD::MHD)
  add_library(MHD::MHD INTERFACE IMPORTED)
  set_property(TARGET MHD::MHD PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${MHD_INCLUDE_DIR}")
  if (WIN32)
    set_property(TARGET MHD::MHD PROPERTY INTERFACE_LINK_LIBRARIES "${MHD_LIBRARY}" ws2_32)
  else ()
    set_property(TARGET MHD::MHD PROPERTY INTERFACE_LINK_LIBRARIES "${MHD_LIBRARY}")
  endif ()
endif ()
