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

NQ_EXPORT NQAsset* NQBinaryAssetCreate(const uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSET_BINARYASSET_H */
