#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET GIF::GIF)
  set(GIF_FOUND TRUE)
  return ()
endif ()

if (GIF_INCLUDE_DIR AND GIF_LIBRARY)
  set(GIF_FIND_QUIETLY TRUE)
endif ()

find_path(GIF_INCLUDE_DIR
  NAMES gif_lib.h
  )

find_library(GIF_LIBRARY
  NAMES gif
  )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GIF DEFAULT_MSG GIF_INCLUDE_DIR GIF_LIBRARY)

mark_as_advanced(GIF_INCLUDE_DIR GIF_LIBRARY)

if (GIF_FOUND)
  set(GIF_INCLUDE_DIRS "${GIF_INCLUDE_DIR}")
  set(GIF_LIBRARIES "${GIF_LIBRARY}")
endif()

if (GIF_FOUND AND NOT TARGET GIF::GIF)
  add_library(GIF::GIF INTERFACE IMPORTED)
  set_property(TARGET GIF::GIF PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${GIF_INCLUDE_DIR}")
  set_property(TARGET GIF::GIF PROPERTY INTERFACE_LINK_LIBRARIES "${GIF_LIBRARY}")
endif ()
