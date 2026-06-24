#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET Leptonica::Leptonica)
  set(Leptonica_FIND_QUIETLY TRUE)
  set(Leptonica_FOUND TRUE)
  return ()
endif ()

if (Leptonica_INCLUDE_DIR AND Leptonica_LIBRARY)
  set(Leptonica_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_Leptonica leptonica)
endif ()

find_path(Leptonica_INCLUDE_DIR
  NAMES leptonica/allheaders.h
  PATHS
    ${PC_Leptonica_INCLUDEDIR}
    ${PC_Leptonica_INCLUDE_DIR}
  )

set(Leptonica_FIND_LIBRARIES
  leptonica
  )

if (Leptonica_INCLUDE_DIR)
  file(READ "${Leptonica_INCLUDE_DIR}/leptonica/allheaders.h" Leptonica_VERSION_CONTENT)

  string(REGEX MATCH "#define +LIBLEPT_MAJOR_VERSION +([0-9]+)" _dummy "${Leptonica_VERSION_CONTENT}")
  set(Leptonica_MAJOR_VERSION "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +LIBLEPT_MINOR_VERSION +([0-9]+)" _dummy "${Leptonica_VERSION_CONTENT}")
  set(Leptonica_MINOR_VERSION "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define +LIBLEPT_PATCH_VERSION +([0-9]+)" _dummy "${Leptonica_VERSION_CONTENT}")
  set(Leptonica_PATCH_VERSION "${CMAKE_MATCH_1}")

  set(Leptonica_VERSION "${Leptonica_MAJOR_VERSION}.${Leptonica_MINOR_VERSION}.${Leptonica_PATCH_VERSION}")
  list(APPEND Leptonica_FIND_LIBRARIES "leptonica-${Leptonica_VERSION}")
endif ()

find_library(Leptonica_LIBRARY
  NAMES ${Leptonica_FIND_LIBRARIES}
  PATHS
    ${PC_Leptonica_LIBDIR}
    ${PC_Leptonica_LIBRARIES_DIRS}
  )

unset(Leptonica_FIND_LIBRARIES)
mark_as_advanced(Leptonica_INCLUDE_DIR Leptonica_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Leptonica REQUIRED_VARS Leptonica_LIBRARY Leptonica_INCLUDE_DIR VERSION_VAR Leptonica_VERSION)
if (LEPTONICA_FOUND)
  set(Leptonica_FOUND TRUE)
endif ()

if (Leptonica_FOUND)
  set(Leptonica_INCLUDE_DIRS "${Leptonica_INCLUDE_DIR}")
  set(Leptonica_LIBRARIES "${Leptonica_LIBRARY}")
endif()

if (Leptonica_FOUND AND NOT TARGET Leptonica::Leptonica)
  add_library(Leptonica::Leptonica UNKNOWN IMPORTED)
  set_target_properties(Leptonica::Leptonica PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Leptonica_INCLUDE_DIR}")
  set_target_properties(Leptonica::Leptonica PROPERTIES IMPORTED_LOCATION "${Leptonica_LIBRARY}")
endif ()
