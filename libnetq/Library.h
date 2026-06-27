/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LIBRARY_H
#define _LIBNETQ_LIBRARY_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_WIN32_LIBRARY_PATH "PATH"
#define NQ_APPLE_LIBRARY_PATH "DYLD_LIBRARY_PATH"
#define NQ_POSIX_LIBRARY_PATH "LD_LIBRARY_PATH"

#define NQ_WIN32_LIBRARY_EXT ".dll"
#define NQ_APPLE_LIBRARY_EXT ".dylib"
#define NQ_POSIX_LIBRARY_EXT ".so"

#if defined(NQ_OS_WINDOWS)
#define NQ_LIBRARY_PATH NQ_WIN32_LIBRARY_PATH
#define NQ_LIBRARY_EXT NQ_WIN32_LIBRARY_EXT
#elif defined(NQ_OS_DARWIN)
#define NQ_LIBRARY_PATH NQ_APPLE_LIBRARY_PATH
#define NQ_LIBRARY_EXT NQ_APPLE_LIBRARY_EXT
#elif defined(NQ_OS_UNIX)
#define NQ_LIBRARY_PATH NQ_POSIX_LIBRARY_PATH
#define NQ_LIBRARY_EXT NQ_POSIX_LIBRARY_EXT
#endif

typedef void* NQLibrary;
typedef void* NQSymbol;

NQ_EXPORT NQLibrary NQLibraryOpen(const char* path);
NQ_EXPORT void NQLibraryClose(NQLibrary handle);
NQ_EXPORT NQSymbol NQLibraryGetSymbol(NQLibrary handle, const char* name);

typedef struct NQLibraryInfo NQLibraryInfo;
struct NQLibraryInfo {
  const char* filename;
  void* baseAddr;
};

NQ_EXPORT int NQLibraryInfoLoad(NQLibraryInfo*, const void* addr);
NQ_EXPORT void NQLibraryInfoFinalize(NQLibraryInfo*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_LIBRARY_H */
