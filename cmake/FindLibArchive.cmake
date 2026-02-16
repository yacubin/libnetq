#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET LibArchive::LibArchive)
  set(LibArchive_FIND_QUIETLY TRUE)
  set(LibArchive_FOUND TRUE)
  return ()
endif ()

if (LibArchive_INCLUDE_DIR AND LibArchive_LIBRARY)
  set(LibArchive_FIND_QUIETLY TRUE)
endif ()

find_path(LibArchive_INCLUDE_DIR
  NAMES archive.h
  )

find_library(LibArchive_LIBRARY
  NAMES archive archive_static
  )

unset(LibArchive_FIND_LIBRARIES)
mark_as_advanced(LibArchive_INCLUDE_DIR LibArchive_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibArchive DEFAULT_MSG LibArchive_INCLUDE_DIR LibArchive_LIBRARY)
if (LIBARCHIVE_FOUND)
  set(LibArchive_FOUND TRUE)
endif ()

if (LibArchive_FOUND)
  set(LibArchive_INCLUDE_DIRS ${LibArchive_INCLUDE_DIR})
  set(LibArchive_LIBRARIES "${LibArchive_LIBRARY}")
endif()

if (LibArchive_FOUND AND NOT TARGET LibArchive::LibArchive)
  add_library(LibArchive::LibArchive UNKNOWN IMPORTED)
  set_target_properties(LibArchive::LibArchive PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${LibArchive_INCLUDE_DIR}")
  set_target_properties(LibArchive::LibArchive PROPERTIES IMPORTED_LOCATION "${LibArchive_LIBRARY}")
endif ()
