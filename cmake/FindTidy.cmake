#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Tidy::Tidy)
  set(Tidy_FOUND TRUE)
  set(TIDY_FOUND TRUE)
  return ()
endif ()

if (Tidy_INCLUDE_DIR AND Tidy_LIBRARY)
  set(Tidy_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_Tidy tidy)
endif ()

find_path(Tidy_INCLUDE_DIR
  NAMES tidy.h
  PATHS
    ${PC_Tidy_INCLUDEDIR}
    ${PC_Tidy_INCLUDE_DIR}
  )

find_library(Tidy_LIBRARY
  NAMES tidy
  PATHS
    ${PC_Tidy_LIBDIR}
    ${PC_Tidy_LIBRARIES_DIRS}
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Tidy DEFAULT_MSG Tidy_INCLUDE_DIR Tidy_LIBRARY)

mark_as_advanced(Tidy_INCLUDE_DIR Tidy_LIBRARY)

if (Tidy_FOUND OR TIDY_FOUND)
  set(Tidy_FOUND TRUE)
  set(Tidy_INCLUDE_DIRS ${Tidy_INCLUDE_DIR})
  set(Tidy_LIBRARIES "${Tidy_LIBRARY}")
endif()

if (Tidy_FOUND AND NOT TARGET Tidy::Tidy)
  add_library(Tidy::Tidy INTERFACE IMPORTED)
  set_property(TARGET Tidy::Tidy PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${Tidy_INCLUDE_DIR}")
  set_property(TARGET Tidy::Tidy PROPERTY INTERFACE_LINK_LIBRARIES "${Tidy_LIBRARY}")
endif ()
