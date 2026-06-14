#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# Gumbo_FOUND - True if libgumbo was found
# Gumbo_INCLUDE_DIRS - Directories containing libgumbo headers
# Gumbo_LIBRARIES - Libraries to link against libgumbo
# Gumbo_DEFINITIONS - Required compiler definitions for libgumbo
#

if (TARGET Gumbo::Gumbo)
  set(Gumbo_FIND_QUIETLY TRUE)
  set(Gumbo_FOUND TRUE)
  return ()
endif ()

if (Gumbo_INCLUDE_DIR AND Gumbo_LIBRARY)
  set(Gumbo_FIND_QUIETLY TRUE)
endif ()

find_package(PkgConfig)
if (PKGCONFIG_FOUND)
  pkg_check_modules(PC_Gumbo gumbo)
  set(gumbo_DEFINITIONS ${PC_Gumbo_CFLAGS_OTHER})
endif ()

find_path(Gumbo_INCLUDE_DIR
  NAMES gumbo.h
  PATHS
    ${PC_Gumbo_INCLUDEDIR}
    ${PC_Gumbo_INCLUDE_DIRS}
  )
  
find_library(Gumbo_LIBRARY
  NAMES gumbo
  PATHS
    ${PC_Gumbo_LIBDIR}
    ${PC_Gumbo_LIBRARY_DIRS}
  )

mark_as_advanced(Gumbo_INCLUDE_DIR Gumbo_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Gumbo REQUIRED_VARS Gumbo_LIBRARY Gumbo_INCLUDE_DIR)
if (GUMBO_FOUND)
  set(Gumbo_FOUND TRUE)
endif ()

if (Gumbo_FOUND)
  set(Gumbo_INCLUDE_DIRS "${Gumbo_INCLUDE_DIR}")
  set(Gumbo_LIBRARIES "${Gumbo_LIBRARY}")
endif ()

if (Gumbo_FOUND AND NOT TARGET Gumbo::Gumbo)
  add_library(Gumbo::Gumbo UNKNOWN IMPORTED)
  set_target_properties(Gumbo::Gumbo PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${Gumbo_DEFINITIONS}")
  set_target_properties(Gumbo::Gumbo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Gumbo_INCLUDE_DIR}")
  set_target_properties(Gumbo::Gumbo PROPERTIES IMPORTED_LOCATION "${Gumbo_LIBRARY}")
endif ()
