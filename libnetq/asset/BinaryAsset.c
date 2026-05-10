/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQBinaryAsset"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/asset/BinaryAsset.h"

#ifdef NQ_USE_ZLIB_BINARYASSET

#include <zlib.h>

#include <libnetq/String.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/Zip.h>
#include <libnetq/Assert.h>

typedef struct ZipEntry {
  const char* name;
  uint32_t dataOffset;
  uint32_t dataSize;
  uint64_t size;
} ZipEntry;

struct BinaryAsset {
  NQAsset base;
  const uint8_t* data;
  uint32_t size;

  ZipEntry* files;
  uint16_t totalFiles;
};

struct BinaryAssetDir {
  NQAssetDir base;
  struct BinaryAsset* system;
  uint16_t index;
  char dirname[1];
};

enum {
  FILE_NO_ERROR = 0,
  FILE_DATA_ERROR,
  FILE_INIT_ERROR,
  FILE_READ_ERROR,
};

struct BinaryAssetFile {
  NQAssetFile base;
  struct BinaryAsset* system;
  const uint8_t* data;
  uint64_t size;
  int status;
  z_stream zstream;
};

static char* NQStringFromBytes(const uint8_t* data, size_t size)
{
  char* s = (char*)NQMalloc(size + 1);
  memcpy(s, data, size);
  s[size] = '\0';
  return s;
}

static bool ZipEntry_parse(struct ZipEntry* zfile, const uint8_t* start, const uint8_t* end, const uint8_t** pend)
{
  struct ZipCentralDirHeader cdheader;
  if (!ZipCentralDirHeader_parse(&cdheader, start, end, &start))
    return false;

  zfile->dataOffset = cdheader.offset;
  zfile->dataSize = cdheader.compressedSize;
  zfile->size = cdheader.uncompressedSize;
  
  zfile->name = NQStringFromBytes(start, cdheader.nameSize);
  start += cdheader.nameSize;

  start += cdheader.extraFieldSize;
  start += cdheader.commentSize;

  if (pend)
    *pend = start;

  return true;
}

static void ZipEntry_finalize(struct ZipEntry* zfile)
{
  NQFree((void*)zfile->name);
}

static void dirClose(NQAssetDir* dir)
{
  struct BinaryAssetDir* thiz = (struct BinaryAssetDir*)dir;
  NQFree(thiz);
}

static const char* dirReadFileName(NQAssetDir* dir)
{
  struct BinaryAssetDir* thiz = (struct BinaryAssetDir*)dir;
  while (thiz->index < thiz->system->totalFiles) {
    const char* fileName = thiz->system->files[thiz->index++].name;

    if (*thiz->dirname == '\0')
      return fileName;

    const char* s1 = thiz->dirname;
    const char* s2 = fileName;
    for (;;) {
      if (*s1 != *s2) {
        if (*s1 == '\0' && *s2 == '/' && strchr(s2 + 1, '/') == NULL)
          return s2 + 1;
        break;
      }
      s1++;
      s2++;
    }
  }
  return NULL;
}

static const struct NQAssetDirCallbacks s_assetDirCallbacks =
{
  .close = dirClose,
  .readFileName = dirReadFileName,
};

static NQAssetDir* assetOpenDir(NQAsset* asset, const char* dirname)
{
  struct BinaryAsset* system = NQ_CONTAINER_OF(asset, struct BinaryAsset, base);

  size_t n = strlen(dirname);
  struct BinaryAssetDir* thiz = (struct BinaryAssetDir*)NQZeroMalloc(sizeof(*thiz) + n + 1);
  if (thiz == NULL)
    return NULL;

  thiz->base.callbacks = &s_assetDirCallbacks;
  thiz->system = system;
  thiz->index = 0;
  strcpy(thiz->dirname, dirname);

  return &thiz->base;
}

static const ZipEntry* NQAssetFileBinary_findEntry(struct BinaryAsset* system, const char* filename)
{
  uint16_t i;
  for (i = 0; i < system->totalFiles; i++) {
    const ZipEntry* iter = &system->files[i];
    if (!strcmp(iter->name, filename))
      return iter;
  }
  return NULL;
}

static const uint8_t* ZipEntry_getData(struct BinaryAsset* system, const ZipEntry* zfile)
{
  struct ZipDataDescriptor ddesc;
  struct ZipLocalFileHeader lfheader;

  if (zfile->dataOffset > system->size || system->size < (zfile->dataOffset + zfile->dataSize))
    return NULL;

  const uint8_t* end = system->data + system->size;

  const uint8_t* data = system->data + zfile->dataOffset;
  if (!ZipLocalFileHeader_parse(&lfheader, data, end, &data))
    return NULL;

  data += lfheader.nameSize;
  data += lfheader.extraFieldSize;

  if (end < (data + zfile->dataSize))
    return NULL;


  if (lfheader.flags & ZIP_DATA_DESC_FLAG) {
    if (!ZipDataDescriptor_parse(&ddesc, data + zfile->dataSize, end, NULL))
      return NULL;
  }

  return data;
}

static void* zAlloc(void* ctx, uint32_t count, uint32_t size)
{
  (void)ctx;

  return NQMalloc(count * size);
}

static void zFree(void* ctx, void* p)
{
  (void)ctx;

  NQFree(p);
}

static void fileClose(NQAssetFile* file)
{
  struct BinaryAssetFile* thiz = (struct BinaryAssetFile*)file;
  if (thiz->status != FILE_DATA_ERROR && thiz->status != FILE_INIT_ERROR)
    deflateEnd(&thiz->zstream);
  NQFree(thiz);
}

static int64_t fileGetSize(NQAssetFile* file)
{
  struct BinaryAssetFile* thiz = (struct BinaryAssetFile*)file;
  return thiz->size;
}

static int fileRead(NQAssetFile* file, void* buffer, size_t size)
{
  struct BinaryAssetFile* thiz = (struct BinaryAssetFile*)file;
  if (thiz->status != FILE_NO_ERROR)
    return -1;

  thiz->zstream.next_out = (Bytef*)buffer;
  thiz->zstream.avail_out = (uInt)NQGetMin(size, NQ_INT_MAX);

  uLong totalBefore = thiz->zstream.total_out;
  int err = inflate(&thiz->zstream, Z_NO_FLUSH);
  if (err != Z_OK && err != Z_STREAM_END) {
    thiz->status = FILE_READ_ERROR;
    return -1;
  }

  return (int)(thiz->zstream.total_out - totalBefore);
}

static const struct NQAssetFileCallbacks s_fileCallbacks =
{
  .close = fileClose,
  .getSize = fileGetSize,
  .read = fileRead,
};

static NQAssetFile* assetOpenFile(NQAsset* asset, const char* filename, int mode)
{
  struct BinaryAsset* system = NQ_CONTAINER_OF(asset, struct BinaryAsset, base);

  const ZipEntry* entry = NQAssetFileBinary_findEntry(system, filename);
  if (entry == NULL)
    return NULL;

  struct BinaryAssetFile* thiz = (struct BinaryAssetFile*)NQMalloc(sizeof(*thiz));
  if (thiz == NULL)
    return NULL;

  thiz->base.callbacks = &s_fileCallbacks;
  thiz->system = system;
  thiz->size = entry->size;
  thiz->data = ZipEntry_getData(system, entry);
  thiz->status = FILE_NO_ERROR;

  memset(&thiz->zstream, 0, sizeof(thiz->zstream));
  if (thiz->data == NULL)
    thiz->status = FILE_DATA_ERROR;
  else {
    thiz->zstream.zalloc = zAlloc;
    thiz->zstream.zfree = zFree;
    thiz->zstream.data_type = Z_BINARY;
    thiz->zstream.opaque = Z_NULL;
    thiz->zstream.next_in = (Bytef*)thiz->data;
    thiz->zstream.avail_in = (uInt)entry->dataSize;

    if (inflateInit2(&thiz->zstream, -MAX_WBITS) != Z_OK)
      thiz->status = FILE_INIT_ERROR;
  }

  return &thiz->base;
}

static void assetDestroy(NQAsset* asset)
{
  uint16_t n;

  struct BinaryAsset* system = NQ_CONTAINER_OF(asset, struct BinaryAsset, base);

  if (system->files) {
    for (n = 0; n < system->totalFiles; n++)
      ZipEntry_finalize(&system->files[n]);
    NQFree((void*)system->files);
  }

  NQFree(system);
}

static const struct NQAssetCallbacks s_assetCallbacks =
{
  .destroy = assetDestroy,
  .openDir = assetOpenDir,
  .openFile = assetOpenFile,
};

NQAsset* NQBinaryAssetCreate(const uint8_t* data, size_t size)
{
  if (size < 2 || memcmp(data, "PK", 2) != 0)
    return NULL;

  struct ZipEndCentralDir ecdir;
  const uint8_t* ecdirData = ZipEndCentralDir_find(&ecdir, data, size);
  if (!ecdirData)
    return NULL;

  const uint8_t* end = data + size;
  uintptr_t ecdirOffset = ecdirData - data;

  struct BinaryAsset* system = (struct BinaryAsset*)NQZeroMalloc(sizeof(*system));
  if (system == NULL)
    return NULL;

  struct ZipEntry* files = (struct ZipEntry*)NQMalloc(sizeof(struct ZipEntry) * ecdir.totalEntries);

  system->base.callbacks = &s_assetCallbacks;
  system->data = data;
  system->size = (uint32_t)size;
  system->totalFiles = 0;
  system->files = files;

  if (files != NULL && ecdir.centralDirOffset < ecdirOffset) {
    const uint8_t* fileStart = system->data + ecdir.centralDirOffset;
    const uint8_t* fileEnd = fileStart + NQGetMin(ecdir.centralDirSize, ecdirOffset - ecdir.centralDirOffset);
    while (system->totalFiles < ecdir.totalEntries) {
      if (!ZipEntry_parse(files, fileStart, fileEnd, &fileStart))
        break;
      files++;
      system->totalFiles++;
    }
  }

  return &system->base;
}

#else

NQAsset* NQBinaryAssetCreate(const uint8_t* data, size_t size)
{
  return NULL;
}

#endif
