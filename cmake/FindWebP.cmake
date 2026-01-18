#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET WebP::WebP)
  set(WebP_FOUND TRUE)
  set(WEBP_FOUND TRUE)
  return ()
endif ()

if (WebP_INCLUDE_DIR AND WebP_webp_LIBRARY AND WebP_webpdecoder_LIBRARY AND WebP_webpmux_LIBRARY AND WebP_webpdemux_LIBRARY)
  set(WebP_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_WebP libwebp)
endif ()

find_path(WebP_INCLUDE_DIR
  NAMES webp/decode.h
  PATHS
    ${PC_WebP_INCLUDEDIR}
    ${PC_WebP_INCLUDE_DIR}
  )

find_library(WebP_webp_LIBRARY
  NAMES webp
  PATHS
    ${PC_WebP_LIBDIR}
    ${PC_WebP_LIBRARIES_DIRS}
  )

find_library(WebP_webpdecoder_LIBRARY
  NAMES webpdecoder
  PATHS
    ${PC_WebP_LIBDIR}
    ${PC_WebP_LIBRARIES_DIRS}
  )

find_library(WebP_webpmux_LIBRARY
  NAMES webpdemux
  PATHS
    ${PC_WebP_LIBDIR}
    ${PC_WebP_LIBRARIES_DIRS}
  )

find_library(WebP_webpdemux_LIBRARY
  NAMES webpdemux
  PATHS
    ${PC_WebP_LIBDIR}
    ${PC_WebP_LIBRARIES_DIRS}
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WebP DEFAULT_MSG
  WebP_INCLUDE_DIR
  WebP_webp_LIBRARY
  WebP_webpdecoder_LIBRARY
  WebP_webpmux_LIBRARY
  WebP_webpdemux_LIBRARY
  )

mark_as_advanced(
  WebP_INCLUDE_DIR
  WebP_webp_LIBRARY
  WebP_webpdecoder_LIBRARY
  WebP_webpmux_LIBRARY
  WebP_webpdemux_LIBRARY
  )

if (WEBP_FOUND OR WebP_FOUND)
  set(WebP_FOUND TRUE)
  set(WebP_INCLUDE_DIRS ${WebP_INCLUDE_DIR})
  set(WebP_LIBRARIES ${WebP_webp_LIBRARY} ${WebP_webpdecoder_LIBRARY} ${WebP_webpmux_LIBRARY} ${WebP_webpdemux_LIBRARY})
endif()

if (WebP_FOUND AND NOT TARGET WebP::WebP)
  add_library(WebP::WebP INTERFACE IMPORTED)
  set_property(TARGET WebP::WebP PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${WebP_INCLUDE_DIRS})
  set_property(TARGET WebP::WebP PROPERTY INTERFACE_LINK_LIBRARIES ${WebP_LIBRARIES})
endif ()
