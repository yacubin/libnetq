/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQFileSystemAsset"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/asset/FileSystemAsset.h"

#include <libnetq/String.h>
#include <libnetq/Path.h>
#include <libnetq/Limits.h>
#include <libnetq/Malloc.h>
#include <libnetq/FileHandle.h>
#include <libnetq/StringPrint.h>
#include <libnetq/Assert.h>
#include <libnetq/Log.h>

struct FileSystemAsset {
  NQAsset base;
  uint16_t pathLength;
  char pathCharacters[1];
};

struct FileSystemAssetFile {
  NQAssetFile base;
  NQFileHandle handle;
};

static NQAssetDir* fileSystemAssetOpenDir(NQAsset* asset, const char* dirname)
{
  NQ_ASSERT_NOT_REACHED();
  return NULL;
}

static void fileClose(NQAssetFile* file)
{
  struct FileSystemAssetFile* thiz = (struct FileSystemAssetFile*)file;
  NQFileClose(thiz->handle);
  NQFree(thiz);
}

static int64_t fileGetSize(NQAssetFile* file)
{
  struct FileSystemAssetFile* thiz = (struct FileSystemAssetFile*)file;
  return (int64_t)NQFileGetSize(thiz->handle);
}

static int fileRead(NQAssetFile* file, void* buffer, size_t size)
{
  struct FileSystemAssetFile* thiz = (struct FileSystemAssetFile*)file;
  return NQFileRead(thiz->handle, buffer, size);
}

static const struct NQAssetFileCallbacks s_fileCallbacks =
{
  .close = fileClose,
  .getSize = fileGetSize,
  .read = fileRead,
};

static NQAssetFile* fileSystemAssetOpenFile(NQAsset* asset, const char* filename, int mode)
{
  struct FileSystemAsset* fsAsset = NQ_CONTAINER_OF(asset, struct FileSystemAsset, base);

  // TODO: Replace to NQPathJoin
  NQStringPrint fullpath;
  NQStringPrint_init(&fullpath);
  NQStringPrint_printf(&fullpath, "%s/%s", fsAsset->pathCharacters, filename);

  NQFileHandle handle = NQFileOpen(NQStringPrint_characters(&fullpath), NQ_FOPEN_READ);
  NQStringPrint_finalize(&fullpath);

  if (NQIsFileInvalid(handle)) {
    return NULL;
  }

  struct FileSystemAssetFile* thiz = (struct FileSystemAssetFile*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    NQFileClose(handle);
    return NULL;
  }

  thiz->base.callbacks = &s_fileCallbacks;
  thiz->handle = handle;

  return &thiz->base;
}

static void fileSystemAssetDestroy(NQAsset* asset)
{
  struct FileSystemAsset* thiz = NQ_CONTAINER_OF(asset, struct FileSystemAsset, base);

  NQFree((void*)thiz);
}

static const struct NQAssetCallbacks s_fileSystemAssetCallbacks =
{
  .destroy = fileSystemAssetDestroy,
  .openDir = fileSystemAssetOpenDir,
  .openFile = fileSystemAssetOpenFile,
};

NQAsset* NQFileSystemAssetCreate(const char* dirname)
{
  size_t len = strlen(dirname);
  if (len > NQ_UINT16_MAX) {
    NQ_LOGE("The dirname '%s' is huge", dirname);
    return NULL;
  }

  struct FileSystemAsset* thiz = (struct FileSystemAsset*)NQMalloc(sizeof(*thiz) + len);
  if (thiz == NULL) {
    NQ_LOGE("No memory for '%s'", dirname);
    return NULL;
  }

  thiz->base.callbacks = &s_fileSystemAssetCallbacks;

  memcpy(thiz->pathCharacters, dirname, len + 1);
  if (len > 0 && NQIsPathDelimiter(thiz->pathCharacters[len - 1]))
    thiz->pathCharacters[--len] = '\0';
  thiz->pathLength = (uint16_t)len;

  return &thiz->base;
}
