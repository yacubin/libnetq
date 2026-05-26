#
# MIT License
#
# Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
#
# Permission is granted to use, copy, modify, and distribute this software
# under the MIT License. See LICENSE file for details.
#

if (TARGET BCrypt::BCrypt)
  set(BCrypt_FIND_QUIETLY TRUE)
  set(BCrypt_FOUND TRUE)
  return ()
endif ()

if (BCrypt_INCLUDE_DIR AND BCrypt_LIBRARY)
  set(BCrypt_FIND_QUIETLY TRUE)
endif ()

find_path(BCrypt_INCLUDE_DIR
  NAMES
    bcrypt.h
  PATHS
    ${PC_BCrypt_INCLUDEDIR}
    ${PC_BCrypt_INCLUDE_DIRS}
  )

find_library(BCrypt_LIBRARY
  NAMES
    bcrypt
  PATHS
    ${PC_BCrypt_LIBDIR}
    ${PC_BCrypt_LIBRARY_DIRS}
  )

mark_as_advanced(BCrypt_INCLUDE_DIR BCrypt_LIBRARY)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BCrypt DEFAULT_MSG BCrypt_INCLUDE_DIR BCrypt_LIBRARY)
if (BCRYPT_FOUND)
  set(BCrypt_FOUND TRUE)
endif ()

if (BCrypt_FOUND)
  set(BCrypt_INCLUDE_DIRS" ${BCrypt_INCLUDE_DIR}")
  set(BCrypt_LIBRARIES "${BCrypt_LIBRARY}")
endif ()

if (BCrypt_FOUND AND NOT TARGET BCrypt::BCrypt)
  add_library(BCrypt::BCrypt UNKNOWN IMPORTED)
  set_target_properties(BCrypt::BCrypt PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${BCrypt_INCLUDE_DIR}")
  set_target_properties(BCrypt::BCrypt PROPERTIES IMPORTED_LOCATION "${BCrypt_LIBRARY}")
endif ()
