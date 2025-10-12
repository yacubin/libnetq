/*
 * MIT License
 *
 * Copyright (c) 2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSET_FILESYSTEMASSET_H
#define _LIBNETQ_ASSET_FILESYSTEMASSET_H

#include <libnetq/Asset.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQFileSystemAsset NQFileSystemAsset;

NQ_EXPORT NQFileSystemAsset* NQFileSystemAsset_create(const char* dirname);
NQ_EXPORT void NQFileSystemAsset_destroy(NQFileSystemAsset*);
NQ_EXPORT NQAssetDir* NQFileSystemAsset_openDir(NQFileSystemAsset*, const char* dirname);
NQ_EXPORT NQAssetFile* NQFileSystemAsset_openFile(NQFileSystemAsset*, const char* filename, int mode);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_FILESYSTEMASSET_H */
