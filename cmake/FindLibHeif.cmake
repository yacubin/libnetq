#
# MIT License
#
# Copyright (c) 2022-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (LibHeif_LIBRARIES AND LibHeif_INCLUDE_DIRS)
  set(LibHeif_FIND_QUIETLY TRUE)
  set(LibHeif_FOUND TRUE)
  return ()
endif ()

find_package(PkgConfig QUIET)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_LibHeif libheif)
  set(LibHeif_DEFINITIONS ${PC_LibHeif_CFLAGS_OTHER})
endif ()

find_path(LibHeif_INCLUDE_DIR
  NAMES
    libheif/heif.h
  PATHS
    ${PC_LibHeif_INCLUDEDIR}
    ${PC_LibHeif_INCLUDE_DIRS}
  )
  
find_library(LibHeif_LIBRARY
  NAMES
    heif libheif
  PATHS
    ${PC_LibHeif_LIBDIR}
    ${PC_LibHeif_LIBRARY_DIRS}
  )

mark_as_advanced(LibHeif_INCLUDE_DIR LibHeif_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibHeif DEFAULT_MSG LibHeif_INCLUDE_DIR LibHeif_LIBRARY)
if(LIBHEIF_FOUND)
  set(LibHeif_FOUND TRUE)
endif ()

if(LibHeif_FOUND)
  set(LibHeif_INCLUDE_DIRS ${LibHeif_INCLUDE_DIR})
  set(LibHeif_LIBRARIES ${LibHeif_LIBRARY})
endif()

if (LibHeif_FOUND AND NOT TARGET LibHeif::LibHeif)
  add_library(LibHeif::LibHeif UNKNOWN IMPORTED)
  set_target_properties(LibHeif::LibHeif PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibHeif_INCLUDE_DIR}")
  set_target_properties(LibHeif::LibHeif PROPERTIES IMPORTED_LOCATION "${LibHeif_LIBRARY}")
endif ()
