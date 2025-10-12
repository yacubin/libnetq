/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSET_BINARYASSET_H
#define _LIBNETQ_ASSET_BINARYASSET_H

#include <libnetq/Asset.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQBinaryAsset NQBinaryAsset;

NQ_EXPORT NQBinaryAsset* NQBinaryAsset_create(const uint8_t* data, size_t size);
NQ_EXPORT void NQBinaryAsset_destroy(NQBinaryAsset*);
NQ_EXPORT NQAssetDir* NQAssetBinary_openDir(NQBinaryAsset*, const char* dirname);
NQ_EXPORT NQAssetFile* NQAssetBinary_openFile(NQBinaryAsset*, const char* filename, int mode);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_BINARYASSET_H */
