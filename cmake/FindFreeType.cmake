#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#
# FreeType_FOUND - True if libfreetype was found
# FreeType_INCLUDE_DIRS - Directories containing libfreetype headers
# FreeType_LIBRARIES - Libraries to link against libfreetype
# FreeType_DEFINITIONS - Required compiler definitions for libfreetype
#

if (TARGET FreeType::FreeType)
  set(FreeType_FIND_QUIETLY TRUE)
  set(FreeType_FOUND TRUE)
  return ()
endif ()

if (FreeType_INCLUDE_DIR AND FreeType_LIBRARY)
  set(FreeType_FIND_QUIETLY TRUE)
endif ()

find_path(FreeType_INCLUDE_DIR
  NAMES ft2build.h freetype/config/ftheader.h
  PATHS
    ${PC_FreeType_INCLUDEDIR}
    ${PC_FreeType_INCLUDE_DIRS}
  PATH_SUFFIXES
    freetype
    freetype2
  )
  
find_library(FreeType_LIBRARY
  NAMES freetype freetype2
  PATHS
    ${PC_FreeType_LIBDIR}
    ${PC_FreeType_LIBRARY_DIRS}
  )

mark_as_advanced(FreeType_INCLUDE_DIR FreeType_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FreeType REQUIRED_VARS FreeType_LIBRARY FreeType_INCLUDE_DIR)
if (FREETYPE_FOUND)
  set(FreeType_FOUND TRUE)
endif ()

if (FreeType_FOUND)
  set(FreeType_INCLUDE_DIRS ${FreeType_INCLUDE_DIR})
  set(FreeType_LIBRARIES ${FreeType_LIBRARY})
endif ()

if (FreeType_FOUND AND NOT TARGET FreeType::FreeType)
  add_library(FreeType::FreeType UNKNOWN IMPORTED)
  set_target_properties(FreeType::FreeType PROPERTIES INTERFACE_COMPILE_DEFINITIONS "${FreeType_DEFINITIONS}")
  set_target_properties(FreeType::FreeType PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${FreeType_INCLUDE_DIR}")
  set_target_properties(FreeType::FreeType PROPERTIES IMPORTED_LOCATION "${FreeType_LIBRARY}")
endif ()
