/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQDir"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Dir.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/OS.h>
#include <libnetq/Malloc.h>
#include <libnetq/Path.h>
#include <libnetq/Assert.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
enum {
  NQ_DIR_OPEN_MODE,
  NQ_DIR_NEXT_MODE,
  NQ_DIR_EOF_MODE,
};
#endif

#ifdef NQ_OS_UNIX
#include <dirent.h>
#endif

extern const NQObjectClass __NQDirClass;

struct NQDir {
  const NQObjectClass* class;

#ifdef NQ_OS_WINDOWS
  HANDLE handle;
  WIN32_FIND_DATAW data;
  uint8_t mode;
  char path[(MAX_PATH * 5) / 2]; /* 1/2 */
#endif

#ifdef NQ_OS_UNIX
  DIR* handle;
  struct dirent* dp;
#endif
};

NQDir* NQDir_open(const char* path)
{
  NQDir* dir;

#ifdef NQ_OS_WINDOWS
  WCHAR winpath[MAX_PATH];
  size_t n = NQWinPathFrom(winpath, MAX_PATH, path);
  if (n == 0 || n > (MAX_PATH - 3))
    return NULL;

  if (winpath[n - 1] != NQ_WINPATH_DELIMITER)
    winpath[n++] = NQ_WINPATH_DELIMITER;
  winpath[n++] = '*';
  winpath[n++] = '\0';

  dir = (NQDir*)NQMalloc(sizeof(NQDir));
  if (dir == NULL)
    return NULL;

  dir->class = &__NQDirClass;
  dir->handle = FindFirstFileW(winpath, &dir->data);
  if (dir->handle != INVALID_HANDLE_VALUE) {
    dir->mode = NQ_DIR_OPEN_MODE;
    NQPathFrom((char*)dir->path, sizeof(dir->path), dir->data.cFileName);
    return dir;
  }

  NQFree(dir);
#endif

#ifdef NQ_OS_UNIX
  DIR* handle = opendir(path);
  if (handle == NULL)
    return NULL;

  struct dirent* dp = readdir(handle);
  if (dp != NULL) {
    dir = (NQDir*)NQMalloc(sizeof(NQDir));
    if (dir != NULL) {
      dir->class = &__NQDirClass;
      dir->handle = handle;
      dir->dp = dp;
      return dir;
    }
  }

  closedir(handle);
#endif

  return NULL;
}

bool NQDir_next(NQDir* dir)
{
#ifdef NQ_OS_WINDOWS
  if (dir->mode == NQ_DIR_EOF_MODE)
    return false;

  if (dir->mode == NQ_DIR_OPEN_MODE) {
    dir->mode = NQ_DIR_NEXT_MODE;
    return true;
  }

  if (FindNextFileW(dir->handle, &dir->data)) {
    NQPathFrom((char*)dir->path, sizeof(dir->path), dir->data.cFileName);
    return true;
  }
  dir->mode = NQ_DIR_EOF_MODE;
#endif

#ifdef NQ_OS_UNIX
  if (dir->dp == NULL)
    return NULL;

  dir->dp = readdir(dir->handle);
  if (dir->dp != NULL)
    return true;
#endif

  return false;
}

void NQDir_close(NQDir* dir)
{
#ifdef NQ_OS_WINDOWS
  FindClose(dir->handle);
#endif

#ifdef NQ_OS_UNIX
  closedir(dir->handle);
#endif

  NQFree(dir);
}

const char* NQDir_name(NQDir* dir)
{
#ifdef NQ_OS_WINDOWS
  if (dir->mode == NQ_DIR_NEXT_MODE)
    return dir->path;
#endif

#ifdef NQ_OS_UNIX
  if (dir->dp != NULL)
    return dir->dp->d_name;
#endif

  return NULL;
}

bool NQDir_isFile(NQDir* dir)
{
#ifdef NQ_OS_WINDOWS
  if (dir->mode == NQ_DIR_NEXT_MODE)
    return dir->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? false : true;
#endif

#ifdef NQ_OS_UNIX
  if (dir->dp != NULL)
    return dir->dp->d_type == DT_REG;
#endif

  return false;
}

bool NQDir_isDirectory(NQDir* dir)
{
#ifdef NQ_OS_WINDOWS
  if (dir->mode == NQ_DIR_NEXT_MODE)
    return dir->data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? true : false;
#endif

#ifdef NQ_OS_UNIX
  if (dir->dp != NULL)
    return dir->dp->d_type == DT_DIR;
#endif

  return false;
}

const NQObjectClass __NQDirClass = {
  NQDirObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQDir_close,
};
