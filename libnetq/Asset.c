/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQAsset"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/Asset.h"

#include <libnetq/CStrBase.h>
#include <libnetq/Malloc.h>
#include <libnetq/asset/PlatformAsset.h>
#include <libnetq/asset/BinaryAsset.h>
#include <libnetq/asset/FileSystemAsset.h>

struct NQAsset {
  NQBinaryAsset* binary;
  NQAssetHandle platform;
  NQFileSystemAsset* filesystem;
};

static void NQAsset_init(NQAsset* thiz, const void* binary, const char* dirname, NQAssetHandle handle)
{
  uint32_t size = 0;
  const uint8_t* data = NULL;

  if (binary != NULL && !memcmp(binary, "AST", 3)) {
    data = (uint8_t*)binary + 4;
    memcpy(&size, data, sizeof(size));
    data += sizeof(size);
  }

  thiz->binary = (data && size) ? NQBinaryAsset_create(data, size) : NULL;

  if (handle) {
#ifdef NQ_OS_MACOSX
    CFURLRef url = CFBundleCopyBundleURL(handle);
    if (url != NULL) {
      char path[1024];
      if (CFURLGetFileSystemRepresentation(url, true, (UInt8*)path, sizeof(path))) {
        strcat(path, "/Contents/MacOS/");
      }
      CFRelease(url);
      thiz->platform = (CFBundleRef)CFRetain(handle);
    }
#else
    thiz->platform = handle;
#endif
  }
  else {
    thiz->platform = NULL;
  }

  thiz->filesystem = dirname ? NQFileSystemAsset_create(dirname) : NULL;
}

NQAsset* NQAsset_create(const void* binary, const char* dirname, NQAssetHandle handle)
{
  NQAsset* thiz = (NQAsset*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL) {
    return NULL;
  }

  NQAsset_init(thiz, binary, dirname, handle);
  return thiz;
}

static void NQAsset_finalize(NQAsset* thiz)
{
  if (thiz->binary) {
    NQBinaryAsset_destroy(thiz->binary);
    thiz->binary = NULL;
  }

  if (thiz->platform) {
#ifdef NQ_OS_MACOSX
    CFRelease(thiz->platform);
#endif
    thiz->platform = NULL;
  }

  if (thiz->filesystem) {
    NQFileSystemAsset_destroy(thiz->filesystem);
    thiz->filesystem = NULL;
  }
}

void NQAsset_destroy(NQAsset* thiz)
{
  NQAsset_finalize(thiz);
  NQFree(thiz);
}

NQAssetDir* NQAsset_openDir(NQAsset* thiz, const char* dirname)
{
  NQAssetDir* dir;

  if (thiz->binary) {
    dir = NQAssetBinary_openDir(thiz->binary, dirname);
    if (dir)
      return dir;
  }

  if (thiz->platform) {
    dir = NQPlatformAsset_openDir(thiz->platform, dirname);
    if (dir)
      return dir;
  }

  if (thiz->filesystem) {
    dir = NQFileSystemAsset_openDir(thiz->filesystem, dirname);
    if (dir)
      return dir;
  }

  return NULL;
}

NQAssetFile* NQAsset_openFile(NQAsset* thiz, const char* filename, int mode)
{
  NQAssetFile* file;

  if (thiz->binary) {
    file = NQAssetBinary_openFile(thiz->binary, filename, mode);
    if (file)
      return file;
  }

  if (thiz->platform) {
    file = NQPlatformAsset_openFile(thiz->platform, filename, mode);
    if (file)
      return file;
  }

  if (thiz->filesystem) {
    file = NQFileSystemAsset_openFile(thiz->filesystem, filename, mode);
    if (file)
      return file;
  }

  return NULL;
}

static struct NQAsset s_asset =
{
  NULL,
  NULL,
  NULL,
};

void NQAssetInitialize(const void* binary, const char* dirname, NQAssetHandle handle)
{
  NQAsset_init(&s_asset, binary, dirname, handle);
}

void NQAssetShutdown(void)
{
  NQAsset_finalize(&s_asset);
}

NQAssetDir* NQAssetDir_open(const char* dirname)
{
  return NQAsset_openDir(&s_asset, dirname);
}

void NQAssetDir_close(NQAssetDir* thiz)
{
  thiz->callbacks->close(thiz);
}

const char* NQAssetDir_readFileName(NQAssetDir* thiz)
{
  return thiz->callbacks->readFileName(thiz);
}

NQAssetFile* NQAssetFile_open(const char* filename, int mode)
{
  return NQAsset_openFile(&s_asset, filename, mode);
}

void NQAssetFile_close(NQAssetFile* thiz)
{
  thiz->callbacks->close(thiz);
}

int64_t NQAssetFile_getSize(NQAssetFile* thiz)
{
  return thiz->callbacks->getSize(thiz);
}

int NQAssetFile_read(NQAssetFile* thiz, void* buffer, size_t size)
{
  return thiz->callbacks->read(thiz, buffer, size);
}
