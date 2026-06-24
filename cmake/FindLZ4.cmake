#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LZ4::LZ4)
  set(LZ4_FIND_QUIETLY TRUE)
  set(LZ4_FOUND TRUE)
  return ()
endif ()

if (LZ4_INCLUDE_DIR AND LZ4_LIBRARY)
  set(LZ4_FIND_QUIETLY TRUE)
endif ()

find_path(LZ4_INCLUDE_DIR
  NAMES lz4.h
  )

find_library(LZ4_LIBRARY
  NAMES lz4
  )

mark_as_advanced(LZ4_INCLUDE_DIR LZ4_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LZ4 REQUIRED_VARS LZ4_LIBRARY LZ4_INCLUDE_DIR)

if (LZ4_FOUND)
  set(LZ4_INCLUDE_DIRS "${LZ4_INCLUDE_DIR}")
  set(LZ4_LIBRARIES "${LZ4_LIBRARY}")
endif()

if (LZ4_FOUND AND NOT TARGET LZ4::LZ4)
  add_library(LZ4::LZ4 UNKNOWN IMPORTED)
  set_target_properties(LZ4::LZ4 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LZ4_INCLUDE_DIR}")
  set_target_properties(LZ4::LZ4 PROPERTIES IMPORTED_LOCATION "${LZ4_LIBRARY}")
endif ()
