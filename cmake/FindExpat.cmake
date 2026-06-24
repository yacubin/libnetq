#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# Expat_FOUND - True if libexpat was found
# Expat_INCLUDE_DIRS - Directories containing libexpat headers
# Expat_LIBRARIES - Libraries to link against libexpat
# Expat_DEFINITIONS - Required compiler definitions for libexpat
#

if (TARGET Expat::Expat)
  set(Expat_FIND_QUIETLY TRUE)
  set(Expat_FOUND TRUE)
  return ()
endif ()

if (Expat_INCLUDE_DIR AND Expat_LIBRARY)
  set(Expat_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig QUIET)
if (PkgConfig_FOUND)
  pkg_check_modules(PC_Expat Expat)
  set(Expat_DEFINITIONS ${PC_Expat_CFLAGS_OTHER})
endif ()

find_path(Expat_INCLUDE_DIR
  NAMES expat.h
  PATHS
    ${PC_Expat_INCLUDEDIR}
    ${PC_Expat_INCLUDE_DIR}
  PATH_SUFFIXES
    expat
  )

find_library(Expat_LIBRARY
  NAMES expat expatd
  PATHS
    ${PC_Expat_LIBDIR}
    ${PC_Expat_LIBRARY_DIRS}
  )

mark_as_advanced(Expat_INCLUDE_DIR Expat_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Expat REQUIRED_VARS Expat_LIBRARY Expat_INCLUDE_DIR)
if (EXPAT_FOUND)
  set(Expat_FOUND TRUE)
endif ()

if (Expat_FOUND)
  set(Expat_INCLUDE_DIRS "${Expat_INCLUDE_DIR}")
  set(Expat_LIBRARIES "${Expat_LIBRARY}")
endif ()

if (Expat_FOUND AND NOT TARGET Expat::Expat)
  add_library(Expat::Expat UNKNOWN IMPORTED)
  set_target_properties(Expat::Expat PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Expat_DEFINITIONS}")
  set_target_properties(Expat::Expat PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Expat_INCLUDE_DIR}")
  set_target_properties(Expat::Expat PROPERTIES IMPORTED_LOCATION "${Expat_LIBRARY}")
endif ()
