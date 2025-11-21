/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSET_H
#define _LIBNETQ_ASSET_H

#include <libnetq/Basic.h>
#include <libnetq/Array.h>

#if defined(NQ_OS_ANDROID)
#include <android/asset_manager.h>
typedef AAssetManager* NQAssetHandle;
#elif defined(NQ_OS_WINDOWS)
#include <windows.h>
typedef HINSTANCE NQAssetHandle;
#elif defined(NQ_OS_MACOSX)
#include <CoreFoundation/CFBundle.h>
typedef CFBundleRef NQAssetHandle;
#else
typedef void* NQAssetHandle;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQAsset NQAsset;
typedef struct NQAssetDir NQAssetDir;
typedef struct NQAssetFile NQAssetFile;

NQ_EXPORT void NQAsset_destroy(NQAsset*);
NQ_EXPORT NQAssetDir* NQAsset_openDir(NQAsset*, const char* dirname);
NQ_EXPORT NQAssetFile* NQAsset_openFile(NQAsset*, const char* filename, int mode);
NQ_EXPORT NQUint8Array* NQAssetFile_loadBytes(NQAsset* thiz, const char* filename);

NQ_EXPORT NQAssetDir* NQAssetDir_open(const char* dirname);
NQ_EXPORT void NQAssetDir_close(NQAssetDir* thiz);
NQ_EXPORT const char* NQAssetDir_readFileName(NQAssetDir* thiz);

NQ_EXPORT NQAssetFile* NQAssetFile_open(const char* filename, int mode);
NQ_EXPORT void NQAssetFile_close(NQAssetFile* thiz);
NQ_EXPORT int64_t NQAssetFile_getSize(NQAssetFile* thiz);
NQ_EXPORT int NQAssetFile_read(NQAssetFile* thiz, void* buffer, size_t size);

struct NQAsset {
  const struct NQAssetCallbacks* callbacks;
};

struct NQAssetCallbacks {
  void (*destroy) (NQAsset*);
  NQAssetDir* (*openDir) (NQAsset*, const char* dirname);
  NQAssetFile* (*openFile) (NQAsset*, const char* filename, int mode);
};

struct NQAssetDir {
  const struct NQAssetDirCallbacks* callbacks;
};

struct NQAssetDirCallbacks {
  void (*close) (NQAssetDir*);
  const char* (*readFileName) (NQAssetDir*);
};

struct NQAssetFile {
  const struct NQAssetFileCallbacks* callbacks;
};

struct NQAssetFileCallbacks {
  void (*close) (NQAssetFile*);
  int64_t (*getSize) (NQAssetFile*);
  int (*read) (NQAssetFile*, void* buffer, size_t size);
};

NQ_EXPORT void NQAssetInitialize(const void* binary, const char* dirname, NQAssetHandle handle);
NQ_EXPORT void NQAssetShutdown(void);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_H */
