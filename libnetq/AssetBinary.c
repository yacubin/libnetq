/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQAssetBinary"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/AssetBinary.h"

#include <libnetq/String.h>
#include <libnetq/Limits.h>
#include <libnetq/Math.h>
#include <libnetq/Malloc.h>
#include <libnetq/Zip.h>
#include <libnetq/ObjectClass.h>
#include <libnetq/Assert.h>

#if WITH_ZLIB
#include <zlib.h>

extern const NQObjectClass __NQAssetBinaryClass;
extern const NQObjectClass __NQAssetDirBinaryClass;
extern const NQObjectClass __NQAssetFileBinaryClass;

typedef struct ZipEntry {
  const char* name;
  uint32_t dataOffset;
  uint32_t dataSize;
  uint64_t size;
} ZipEntry;

struct NQAssetSystemBinary {
  const NQObjectClass* class;
  const uint8_t* data;
  uint32_t size;

  ZipEntry* files;
  uint16_t totalFiles;
};

struct NQAssetDirBinary {
  const NQObjectClass* class;
  NQAssetSystemBinary* system;
  uint16_t index;
  char dirname[1];
};

enum {
  FILE_NO_ERROR = 0,
  FILE_DATA_ERROR,
  FILE_INIT_ERROR,
  FILE_READ_ERROR,
};

struct NQAssetFileBinary {
  const NQObjectClass* class;
  NQAssetSystemBinary* system;
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

NQAssetSystemBinary* NQAssetSystemBinary_create(const uint8_t* data, size_t size)
{  
  if (size < 2 || memcmp(data, "PK", 2) != 0)
    return NULL;
  
  struct ZipEndCentralDir ecdir;
  const uint8_t* ecdirData = ZipEndCentralDir_find(&ecdir, data, size);
  if (!ecdirData)
    return NULL;

  const uint8_t* end = data + size;
  uintptr_t ecdirOffset = ecdirData - data;

  NQAssetSystemBinary* system = (NQAssetSystemBinary*)NQZeroMalloc(sizeof(NQAssetSystemBinary));
  if (system == NULL)
    return NULL;

  struct ZipEntry* files = (struct ZipEntry*)NQMalloc(sizeof(struct ZipEntry) * ecdir.totalEntries);

  system->class = &__NQAssetBinaryClass;
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

  return system;
}

void NQAssetSystemBinary_destroy(NQAssetSystemBinary* system)
{
  uint16_t n;

  if (system->files) {
    for (n = 0; n < system->totalFiles; n++)
      ZipEntry_finalize(&system->files[n]);
    NQFree((void*)system->files);
  }

  NQFree(system);
}

NQAssetDirBinary* NQAssetBinary_openDir(NQAssetSystemBinary* system, const char* dirname)
{
  size_t n = strlen(dirname);
  NQAssetDirBinary* dir = (NQAssetDirBinary*)NQZeroMalloc(sizeof(NQAssetDirBinary) + n + 1);
  if (dir == NULL)
    return NULL;

  dir->class = &__NQAssetDirBinaryClass;
  dir->system = system;
  dir->index = 0;
  strcpy(dir->dirname, dirname);

  return dir;
}

const char* NQAssetDirBinary_readFileName(NQAssetDirBinary* dir)
{
  while (dir->index < dir->system->totalFiles) {
    const char* fileName = dir->system->files[dir->index++].name;

    if (*dir->dirname == '\0')
      return fileName;

    const char* s1 = dir->dirname;
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

void NQAssetDirBinary_close(NQAssetDirBinary* dir)
{
  NQFree(dir);
}

static const ZipEntry* NQAssetFileBinary_findEntry(NQAssetSystemBinary* system, const char* filename)
{
  uint16_t i;
  for (i = 0; i < system->totalFiles; i++) {
    const ZipEntry* iter = &system->files[i];
    if (!strcmp(iter->name, filename))
      return iter;
  }
  return NULL;
}

static const uint8_t* ZipEntry_getData(NQAssetSystemBinary* system, const ZipEntry* zfile)
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

NQAssetFileBinary* NQAssetBinary_openFile(NQAssetSystemBinary* system, const char* filename, int mode)
{
  const ZipEntry* entry = NQAssetFileBinary_findEntry(system, filename);
  if (entry == NULL)
    return NULL;

  NQAssetFileBinary* file = (NQAssetFileBinary*)NQZeroMalloc(sizeof(NQAssetFileBinary));
  if (file == NULL)
    return NULL;

  file->class = &__NQAssetFileBinaryClass;
  file->system = system;
  file->size = entry->size;
  file->data = ZipEntry_getData(system, entry);
  file->status = FILE_NO_ERROR;

  memset(&file->zstream, 0, sizeof(file->zstream));
  if (file->data == NULL)
    file->status = FILE_DATA_ERROR;
  else {
    file->zstream.zalloc = zAlloc;
    file->zstream.zfree = zFree;
    file->zstream.data_type = Z_BINARY;
    file->zstream.opaque = Z_NULL;
    file->zstream.next_in = (Bytef*)file->data;
    file->zstream.avail_in = (uInt)entry->dataSize;

    if (inflateInit2(&file->zstream, -MAX_WBITS) != Z_OK)
      file->status = FILE_INIT_ERROR;
  }

  return file;
}

int64_t NQAssetFileBinary_getSize(NQAssetFileBinary* file)
{
  return file->size;
}

int NQAssetFileBinary_read(NQAssetFileBinary* file, void* buffer, size_t size)
{
  if (file->status != FILE_NO_ERROR)
    return -1;
  
  file->zstream.next_out = (Bytef*)buffer;
  file->zstream.avail_out = (uInt)NQGetMin(size, NQ_INT_MAX);

  uLong totalBefore = file->zstream.total_out;
  int err = inflate(&file->zstream, Z_NO_FLUSH);
  if (err != Z_OK && err != Z_STREAM_END) {
    file->status = FILE_READ_ERROR;
    return -1;
  }

  return (int)(file->zstream.total_out - totalBefore);
}

void NQAssetFileBinary_close(NQAssetFileBinary* file)
{
  if (file->status != FILE_DATA_ERROR && file->status != FILE_INIT_ERROR)
    deflateEnd(&file->zstream);

  NQFree(file);
}

const NQObjectClass __NQAssetBinaryClass = {
  kNQAssetBinaryObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetSystemBinary_destroy,
};

const NQObjectClass __NQAssetDirBinaryClass = {
  kNQAssetDirBinaryObjectType,
  "NQAssetDirBinary",
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetDirBinary_close,
};

const NQObjectClass __NQAssetFileBinaryClass = {
  kNQAssetFileBinaryObjectType,
  "AssetFileBinary",
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetFileBinary_close,
};

#else

NQAssetSystemBinary* NQAssetSystemBinary_create(const uint8_t* data, size_t size)
{
  return NULL;
}

void NQAssetSystemBinary_destroy(NQAssetSystemBinary* system)
{
}

NQAssetDirBinary* NQAssetBinary_openDir(NQAssetSystemBinary* system, const char* dirname)
{
  return NULL;
}

const char* NQAssetDirBinary_readFileName(NQAssetDirBinary* dir)
{
  return NULL;
}

void NQAssetDirBinary_close(NQAssetDirBinary* dir)
{
}

NQAssetFileBinary* NQAssetBinary_openFile(NQAssetSystemBinary* system, const char* filename, int mode)
{
  return NULL;
}

int64_t NQAssetFileBinary_getSize(NQAssetFileBinary* file)
{
  return 0;
}

int NQAssetFileBinary_read(NQAssetFileBinary* file, void* buffer, size_t size)
{
  return -1;
}

void NQAssetFileBinary_close(NQAssetFileBinary* file)
{
}

#endif
