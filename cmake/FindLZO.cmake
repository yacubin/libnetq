#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LZO::LZO)
  set(LZO_FIND_QUIETLY TRUE)
  set(LZO_FOUND TRUE)
  return ()
endif ()

if (LZO_INCLUDE_DIR AND LZO_LIBRARY)
  set(LZO_FIND_QUIETLY TRUE)
endif ()

find_path(LZO_INCLUDE_DIR
  NAMES lzo/lzoconf.h
  )

find_library(LZO_LIBRARY
  NAMES lzo2
  )

mark_as_advanced(LZO_INCLUDE_DIR LZO_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LZO REQUIRED_VARS LZO_LIBRARY LZO_INCLUDE_DIR)

if (LZO_FOUND)
  set(LZO_INCLUDE_DIRS "${LZO_INCLUDE_DIR}")
  set(LZO_LIBRARIES "${LZO_LIBRARY}")
endif()

if (LZO_FOUND AND NOT TARGET ::LZO)
  add_library(LZO::LZO UNKNOWN IMPORTED)
  set_target_properties(LZO::LZO PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LZO_INCLUDE_DIR}")
  set_target_properties(LZO::LZO PROPERTIES IMPORTED_LOCATION "${LZO_LIBRARY}")
endif ()
