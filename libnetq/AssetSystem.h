/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSETSYSTEM_H
#define _LIBNETQ_ASSETSYSTEM_H

#include <libnetq/Asset.h>
#include <libnetq/OS.h>

#ifdef NQ_OS_ANDROID
#include <android/asset_manager.h>
#endif

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

#ifdef NQ_OS_MACOSX
#include <CoreFoundation/CFBundle.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
  
typedef struct NQAssetSystem NQAssetSystem;

#if defined(NQ_OS_ANDROID)
typedef AAssetManager* NQAssetHandle;
#elif defined(NQ_OS_WINDOWS)
typedef HINSTANCE NQAssetHandle;
#elif defined(NQ_OS_MACOSX)
typedef CFBundleRef NQAssetHandle;
#else
typedef void* NQAssetHandle;
#endif

NQ_EXPORT NQAssetSystem* NQAssetSystem_create(NQAssetHandle handle, const uint8_t* data, size_t size);
NQ_EXPORT void NQAssetSystem_destroy(NQAssetSystem* system);

NQ_EXPORT NQAssetDir* NQAssetSystem_openDir(NQAssetSystem* system, const char* dirname);
NQ_EXPORT NQAssetFile* NQAssetSystem_openFile(NQAssetSystem* system, const char* filename, int mode);

NQ_EXPORT NQAssetSystem* NQAssetSystemGetMain();

NQ_EXPORT void NQAssetInitialize(const void* asset, NQAssetHandle handle);
NQ_EXPORT void NQAssetShutdown();

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSETSYSTEM_H */
