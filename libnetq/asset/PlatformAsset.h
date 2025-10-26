/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSET_PLATFORMASSET_H
#define _LIBNETQ_ASSET_PLATFORMASSET_H

#include <libnetq/Asset.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT NQAssetDir* NQPlatformAsset_openDir(NQAssetHandle, const char* dirname);
NQ_EXPORT NQAssetFile* NQPlatformAsset_openFile(NQAssetHandle, const char* filename, int mode);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_PLATFORMASSET_H */
