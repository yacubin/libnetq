/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/Executable.h"

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#include <libnetq/Path.h>
#endif

#ifdef NQ_OS_UNIX
#include <unistd.h>
#include <string.h>
#endif

size_t NQGetCommandLine(char* buffer, size_t n)
{
#ifdef NQ_OS_WINDOWS
  LPWSTR cmdline = GetCommandLineW();
  if (cmdline != NULL)
    return NQPathFrom(buffer, n, cmdline);
#endif

#ifdef NQ_OS_UNIX
  ssize_t i;
  ssize_t size = readlink("/proc/self/cmdline", buffer, n);
  if (size > 0) {
    size--;
    for (i = 0; i < size; i++) {
      if (buffer[i] == 0)
        buffer[i] = ' ';
    }
    return (size_t)size;
  }
#endif

  if (n != 0)
    *buffer = '\0';

  return 0;
}

size_t NQGetCurrentDirectory(char* buffer, size_t n)
{
#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  DWORD length = GetCurrentDirectoryW(MAX_PATH, winpath);
  if (length > 0)
    return NQPathFrom(buffer, n, winpath);
#endif

#ifdef NQ_OS_UNIX
  if (getcwd(buffer, n) != NULL)
    return strlen(buffer);
#endif

  if (n != 0)
    *buffer = '\0';

  return 0;
}

size_t NQGetExecutablePath(char* buffer, size_t n)
{
#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  DWORD length = GetModuleFileNameW(NULL, winpath, MAX_PATH);
  if (length > 0)
    return NQPathFrom(buffer, n, winpath);
#endif

#ifdef NQ_OS_UNIX
  // Linux:   /proc/self/exe
  // FreeBSD: /proc/curproc/file
  // Solaris: /proc/self/path/a.out
  ssize_t size = readlink("/proc/self/exe", buffer, n);
  if (size > 0) {
    buffer[size] = '\0';
    return (size_t)size;
  }
#endif

  if (n != 0)
    *buffer = '\0';

  return 0;
}
