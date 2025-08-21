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

#ifdef __cplusplus
extern "C" {
#endif
  
typedef struct NQAssetDir NQAssetDir;
typedef struct NQAssetFile NQAssetFile;

NQ_EXPORT NQAssetDir* NQAssetDir_open(const char* dirname);
NQ_EXPORT const char* NQAssetDir_readFileName(NQAssetDir* assetDir);
NQ_EXPORT void NQAssetDir_close(NQAssetDir* assetDir);

NQ_EXPORT NQAssetFile* NQAssetFile_open(const char* filename, int mode);
NQ_EXPORT int64_t NQAssetFile_getSize(NQAssetFile* asset);
NQ_EXPORT int NQAssetFile_read(NQAssetFile* assetFile, void* buffer, size_t size);
NQ_EXPORT void NQAssetFile_close(NQAssetFile* assetFile);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_H */
