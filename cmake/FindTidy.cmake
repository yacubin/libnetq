#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Tidy::Tidy)
  set(Tidy_FIND_QUIETLY TRUE)
  set(Tidy_FOUND TRUE)
  return ()
endif ()

if (Tidy_INCLUDE_DIR AND Tidy_LIBRARY)
  set(Tidy_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
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

mark_as_advanced(Tidy_INCLUDE_DIR Tidy_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Tidy DEFAULT_MSG Tidy_INCLUDE_DIR Tidy_LIBRARY)
if (TIDY_FOUND)
  set(Tidy_FOUND TRUE)
endif()

if (Tidy_FOUND)
  set(Tidy_INCLUDE_DIRS ${Tidy_INCLUDE_DIR})
  set(Tidy_LIBRARIES "${Tidy_LIBRARY}")
endif()

if (Tidy_FOUND AND NOT TARGET Tidy::Tidy)
  add_library(Tidy::Tidy UNKNOWN IMPORTED)
  set_target_properties(Tidy::Tidy PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Tidy_INCLUDE_DIR}")
  set_target_properties(Tidy::Tidy PROPERTIES IMPORTED_LOCATION "${Tidy_LIBRARY}")
endif ()
