/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQFileSystem"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/FileSystem.h"

#include <libnetq/FileHandle.h>
#include <libnetq/Path.h>
#include <libnetq/Limits.h>
#include <libnetq/Log.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#include <shlobj.h>
#endif

#if defined(NQ_OS_LINUX) && !defined(PATH_MAX)
#include <linux/limits.h>
#endif

#ifdef NQ_OS_UNIX
#include <sys/stat.h>
#include <unistd.h>
#endif

int64_t NQFileSystem_read(const char* path, uint8_t* data, int64_t size)
{
  int64_t result;
  NQFileHandle handle;

  handle = NQFileOpen(path, NQ_FOPEN_READ);
  if (!NQFileCheck(handle))
    return -1;

  result = NQFileReadn(handle, data, size);
  NQFileClose(handle);

  return result;
}

int64_t NQFileSystem_write(const char* path, const uint8_t* data, int64_t size)
{
  int64_t result;
  NQFileHandle handle;

  handle = NQFileOpen(path, NQ_FOPEN_WRITE);
  if (!NQFileCheck(handle))
    return -1;

  result = NQFileWriten(handle, data, size);
  NQFileClose(handle);

  return result;
}

bool NQFileSystem_mkdir(const char* path, bool recursive)
{
  (void)recursive; // TODO

#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  if (NQGetAbsoluteWinPath(winpath, MAX_PATH, path) == 0)
    return false;
  if (SHCreateDirectoryExW(NULL, winpath, NULL) == ERROR_SUCCESS)
    return true;
  DWORD error = GetLastError();
  if (error == ERROR_FILE_EXISTS || error == ERROR_ALREADY_EXISTS)
    return true;

#endif

#ifdef NQ_OS_UNIX
  if (!access(path, F_OK))
    return true;

  char fullPath[PATH_MAX];
  size_t length = NQGetAbsolutePath(fullPath, sizeof(fullPath), path);
  if (length == 0 || sizeof(fullPath) <= length)
    return false;

  char* p = fullPath + 1;
  if (p[length - 1] == '/')
    p[length - 1] = '\0';
  for (; *p; ++p) {
    if (*p == '/') {
      *p = '\0';
      if (access(fullPath, F_OK)) {
        if (mkdir(fullPath, S_IRWXU))
          return false;
      }
      *p = '/';
    }
  }
  if (!access(fullPath, F_OK))
    return true;
  if (!mkdir(fullPath, S_IRWXU))
    return true;

#endif

  return false;
}

bool NQFileSystem_exists(const char* path)
{
#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  if (NQWinPathFrom(winpath, sizeof(winpath), path) < sizeof(winpath)) {
#ifndef NQ_WIN_FS_EXISTS_V2
    DWORD attr = GetFileAttributesW(winpath);
    if (attr != INVALID_FILE_ATTRIBUTES) //  && !(attr & FILE_ATTRIBUTE_DIRECTORY)
      return true;
#else
    WIN32_FIND_DATAW findFileData;
    HANDLE handle = FindFirstFileW(winpath, &findFileData);
    if(handle != INVALID_HANDLE_VALUE) {
      FindClose(handle);
      return true;
    }
#endif
  }
#endif

#ifdef NQ_OS_UNIX
  if (!access(path, F_OK))
    return true;
#endif

  return false;
}
