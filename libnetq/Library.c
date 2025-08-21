/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Library.h"

#include <libnetq/Path.h>
#include <libnetq/Assert.h>

#if defined(NQ_OS_WIN)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(NQ_OS_WIN)
static NQLibrary openWinLibraryA(const char* path)
{
  NQLibrary handle = LoadLibraryExA(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
  return handle != NULL ? LoadLibraryExA(path, NULL, 0) : handle;
}

static NQLibrary openWinLibraryW(const WCHAR* path)
{
  NQLibrary handle = LoadLibraryExW(path, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
  return handle != NULL ? LoadLibraryExW(path, NULL, 0) : handle;
}
#endif

NQLibrary NQLibraryOpen(const char* path)
{
#if defined(NQ_OS_WIN)
  WCHAR winpath[MAX_PATH];
  NQWinPathFrom(winpath, MAX_PATH, path);
  NQLibrary handle = openWinLibraryW(winpath);
  if (handle == NULL)
    handle = openWinLibraryA(path);
  return handle;
#elif defined(NQ_OS_UNIX)
  return dlopen(path, RTLD_LAZY);
#else
  return NULL;
#endif
}

void NQLibraryClose(NQLibrary handle)
{
#if defined(NQ_OS_WIN)
  FreeLibrary(handle);
#elif defined(NQ_OS_UNIX)
  dlclose(handle);
#endif
}

NQSymbol NQLibraryGetSymbol(NQLibrary handle, const char* name)
{
#if defined(NQ_OS_WIN)
  return GetProcAddress(handle, name);
#elif defined(NQ_OS_UNIX)
  return dlsym(handle, name);
#else
  return NULL;
#endif
}
