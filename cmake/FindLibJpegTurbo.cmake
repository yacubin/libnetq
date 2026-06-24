#
# MIT License
#
# Copyright (c) 2023-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LibJpegTurbo::LibJpegTurbo)
  set(LibJpegTurbo_FIND_QUIETLY TRUE)
  set(LibJpegTurbo_FOUND TRUE)
  return ()
endif ()

if (LibJpegTurbo_LIBRARIES AND LibJpegTurbo_INCLUDE_DIRS)
  set(LibJpegTurbo_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_LibJpegTurbo LibJpegTurbo)
  set(LibJpegTurbo_DEFINITIONS ${PC_LibJpegTurbo_CFLAGS_OTHER})
endif ()

find_path(LibJpegTurbo_INCLUDE_DIR
  NAMES
    jconfig.h turbojpeg.h jpeglib.h
  PATHS
    ${PC_LibJpegTurbo_INCLUDEDIR}
    ${PC_LibJpegTurbo_INCLUDE_DIRS}
  PATH_SUFFIXES
    libjpeg-turbo
  )

#TODO: Version from jconfig.h

find_library(LibJpegTurbo_TurboJpeg_LIBRARY
  NAMES
    turbojpeg
  PATHS
    ${PC_LibJpegTurbo_LIBDIR}
    ${PC_LibJpegTurbo_LIBRARY_DIRS}
  )
  
find_library(LibJpegTurbo_Jpeg_LIBRARY
  NAMES
    jpeg
  PATHS
    ${PC_LibJpegTurbo_LIBDIR}
    ${PC_LibJpegTurbo_LIBRARY_DIRS}
  )

mark_as_advanced(LibJpegTurbo_INCLUDE_DIR LibJpegTurbo_TurboJpeg_LIBRARY LibJpegTurbo_Jpeg_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibJpegTurbo REQUIRED_VARS LibJpegTurbo_TurboJpeg_LIBRARY LibJpegTurbo_INCLUDE_DIR LibJpegTurbo_Jpeg_LIBRARY)
if (LIBJPEGTURBO_FOUND)
  set(LibJpegTurbo_FOUND TRUE)
endif ()

if (LibJpegTurbo_FOUND)
  set(LibJpegTurbo_INCLUDE_DIRS "${LibJpegTurbo_INCLUDE_DIR}")
  set(LibJpegTurbo_LIBRARIES "${LibJpegTurbo_TurboJpeg_LIBRARY}" "${LibJpegTurbo_Jpeg_LIBRARY}")
endif ()

if (LibJpegTurbo_FOUND AND NOT TARGET LibJpegTurbo::LibJpegTurbo)
  add_library(LibJpegTurbo::jpeg UNKNOWN IMPORTED)
  set_target_properties(LibJpegTurbo::jpeg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibJpegTurbo_INCLUDE_DIR}")
  set_target_properties(LibJpegTurbo::jpeg PROPERTIES IMPORTED_LOCATION "${LibJpegTurbo_Jpeg_LIBRARY}")
endif ()

if (LibJpegTurbo_FOUND AND NOT TARGET LibJpegTurbo::LibJpegTurbo)
  add_library(LibJpegTurbo::LibJpegTurbo UNKNOWN IMPORTED)
  set_target_properties(LibJpegTurbo::LibJpegTurbo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibJpegTurbo_INCLUDE_DIR}")
  set_target_properties(LibJpegTurbo::LibJpegTurbo PROPERTIES IMPORTED_LOCATION "${LibJpegTurbo_TurboJpeg_LIBRARY}")
  set_target_properties(LibJpegTurbo::LibJpegTurbo PROPERTIES INTERFACE_LINK_LIBRARIES "LibJpegTurbo::jpeg")
endif ()
