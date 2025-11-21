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

NQ_EXPORT NQAsset* NQFileSystemAssetCreate(const char* dirname);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_FILESYSTEMASSET_H */
