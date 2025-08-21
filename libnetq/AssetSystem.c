/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQAssetSystem"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/AssetSystem.h"

#include <libnetq/ObjectClass.h>
#include <libnetq/AssetBinary.h>
#include <libnetq/Malloc.h>
#include <libnetq/CStrBase.h>

extern const NQObjectClass __NQAssetDirClass;
extern const NQObjectClass __NQAssetFileClass;
extern const NQObjectClass __NQAssetSystemClass;

struct NQAssetSystem {
  const NQObjectClass* class;
  NQAssetSystemBinary* binary;
  NQAssetHandle native;
};

struct NQAssetDir {
  const NQObjectClass* class;
  bool isNative;
  union {
    NQAssetDirBinary* binary;

#if defined(NQ_OS_ANDROID)
    AAssetDir* native;
#endif

  };
};

struct NQAssetFile {
  const NQObjectClass* class;
  bool isNative;
  union {
    NQAssetFileBinary* binary;

#if defined(NQ_OS_ANDROID)
    AAsset* native;
#endif

  };
};

static void NQAssetSystem_init(NQAssetSystem* system, NQAssetHandle native, const uint8_t* data, size_t size)
{
  system->class = &__NQAssetSystemClass;

  if (data != NULL)
    system->binary = NQAssetSystemBinary_create(data, size);

#if defined(NQ_OS_ANDROID)
  system->native = native;
#elif defined(NQ_OS_WINDOWS)
  system->native = native;
#elif defined(NQ_OS_MACOSX)
  CFURLRef url = CFBundleCopyBundleURL(native);
  if (url != NULL) {
    char path[1024];
    if (CFURLGetFileSystemRepresentation(url, true, (UInt8*)path, sizeof(path))) {
      strcat(path, "/Contents/MacOS/");
    }
    CFRelease(url);
    system->native = (CFBundleRef)CFRetain(native);
  }
#endif
}

NQAssetSystem* NQAssetSystem_create(NQAssetHandle handle, const uint8_t* data, size_t size)
{
  NQAssetSystem* system = (NQAssetSystem*)NQZeroMalloc(sizeof(NQAssetSystem));
  if (system == NULL)
    return NULL;

  NQAssetSystem_init(system, handle, data, size);
  return system;
}

static void NQAssetSystem_finalize(NQAssetSystem* system)
{
  if (system->binary != NULL)
    NQAssetSystemBinary_destroy(system->binary);

#ifdef NQ_OS_MACOSX
  if (system->native != NULL)
    CFRelease(system->native);
#endif
}

void NQAssetSystem_destroy(NQAssetSystem* system)
{
  NQAssetSystem_finalize(system);
  NQFree(system);
}

static NQAssetDir* NQAssetSystem_openBinaryDir(NQAssetSystem* system, const char* dirname)
{
  if (system->binary == NULL)
    return NULL;
  
  NQAssetDirBinary* binary = NQAssetBinary_openDir(system->binary, dirname);
  if (binary == NULL)
    return NULL;
  
  NQAssetDir* dir = (NQAssetDir*)NQZeroMalloc(sizeof(NQAssetDir));
  if (dir == NULL)
    NQAssetDirBinary_close(binary);
  else {
    dir->class = &__NQAssetDirClass;
    dir->isNative = false;
    dir->binary = binary;
  }

  return dir;
}

NQAssetDir* NQAssetSystem_openDir(NQAssetSystem* system, const char* dirname)
{
  NQAssetDir* dir = NQAssetSystem_openBinaryDir(system, dirname);
  if (dir == NULL) {

#if defined(NQ_OS_ANDROID)
    AAssetDir* native = AAssetManager_openDir(system->native, dirname);
    if (native != NULL) {
      dir = (NQAssetDir*)NQZeroMalloc(sizeof(NQAssetDir));
      if (dir == NULL)
        AAssetDir_close(native);
      else {
        dir->class = &__NQAssetDirClass;
        dir->isNative = true;
        dir->native = native;
      }
    }
#endif

  }

  return dir;
}

static NQAssetSystem s_asset = {
  &__NQAssetSystemClass,
  NULL,
  NULL
};

NQAssetSystem* NQAssetSystemGetMain()
{
  return &s_asset;
}

NQAssetDir* NQAssetDir_open(const char* dirname)
{
  return NQAssetSystem_openDir(&s_asset, dirname);
}

const char* NQAssetDir_readFileName(NQAssetDir* dir)
{
  if (!dir->isNative)
    return NQAssetDirBinary_readFileName(dir->binary);
  else {
#if defined(NQ_OS_ANDROID)
    return AAssetDir_getNextFileName(dir->native);
#else
    return NULL;
#endif
  }
}

void NQAssetDir_close(NQAssetDir* dir)
{
  if (!dir->isNative)
    NQAssetDirBinary_close(dir->binary);
  else {
#if defined(NQ_OS_ANDROID)
    AAssetDir_close(dir->native);
#endif
  }

  NQFree(dir);
}

static NQAssetFile* NQAssetSystem_openBinaryFile(NQAssetSystem* system, const char* filename, int mode)
{
  if (system->binary == NULL)
    return NULL;

  NQAssetFileBinary* binary = NQAssetBinary_openFile(system->binary, filename, mode);
  if (binary == NULL)
    return NULL;

  NQAssetFile* file = (NQAssetFile*)NQZeroMalloc(sizeof(NQAssetFile));
  if (file == NULL)
    NQAssetFileBinary_close(binary);
  else {
    file->class = &__NQAssetFileClass;
    file->isNative = false;
    file->binary = binary;
  }

  return file;
}

NQAssetFile* NQAssetSystem_openFile(NQAssetSystem* system, const char* filename, int mode)
{
  NQAssetFile* file = NQAssetSystem_openBinaryFile(system, filename, mode);
  if (file == NULL) {

#if defined(NQ_OS_ANDROID)
    AAsset* native = AAssetManager_open(system->native, filename, AASSET_MODE_STREAMING);
    if (native != NULL) {
      file = (NQAssetFile*)NQZeroMalloc(sizeof(NQAssetFile));
      if (file == NULL)
        AAsset_close(native);
      else {
        file->class = &__NQAssetFileClass;
        file->isNative = true;
        file->native = native;
      }
    }
#endif

  }

  return file;
}

NQAssetFile* NQAssetFile_open(const char* filename, int mode)
{
  return NQAssetSystem_openFile(&s_asset, filename, mode);
}

int64_t NQAssetFile_getSize(NQAssetFile* file)
{
  if (!file->isNative)
    return NQAssetFileBinary_getSize(file->binary);
  else {
#if defined(NQ_OS_ANDROID)
#ifdef HAVE_AASSET_GETLENGTH64
    return (int64_t)AAsset_getLength64(file->native);
#else
    // Try dlopen AAsset_getLength64
    return (int64_t)AAsset_getLength(file->native);
#endif
#else
    return -1;
#endif
  }
}

int NQAssetFile_read(NQAssetFile* file, void* buffer, size_t size)
{
  if (!file->isNative)
    return NQAssetFileBinary_read(file->binary, buffer, size);
  else {
#if defined(NQ_OS_ANDROID)
    return AAsset_read(file->native, buffer, size);
#else
    return -1;
#endif
  }
}

void NQAssetFile_close(NQAssetFile* file)
{
  if (!file->isNative)
    NQAssetFileBinary_close(file->binary);
  else {
#if defined(NQ_OS_ANDROID)
    AAsset_close(file->native);
#endif
  }

  NQFree(file);
}

void NQAssetInitialize(const void* asset, NQAssetHandle handle)
{
  uint32_t size = 0;
  const uint8_t* data = NULL;

  if (asset != NULL && !memcmp(asset, "AST", 3)) {
    data = (uint8_t*)asset + 4;
    memcpy(&size, data, sizeof(size));
    data += sizeof(size);
  }

  NQAssetSystem_init(&s_asset, handle, data, size);
}

void NQAssetShutdown()
{
  NQAssetSystem_finalize(&s_asset);
}

const NQObjectClass __NQAssetDirClass = {
  NQAssetDirObjectType,
  "NQAssetDir",
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetDir_close,
};

const NQObjectClass __NQAssetFileClass = {
  NQAssetFileObjectType,
  "NQAssetFile",
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetFile_close,
};

const NQObjectClass __NQAssetSystemClass = {
  NQAssetSystemObjectType,
  NQ_CLASS_NAME,
  NQ_VERSION_CODE,
  (NQObjectReleaseCallback)NQAssetSystem_destroy,
};
