/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ASSETBINARY_H
#define _LIBNETQ_ASSETBINARY_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQAssetSystemBinary NQAssetSystemBinary;
typedef struct NQAssetDirBinary NQAssetDirBinary;
typedef struct NQAssetFileBinary NQAssetFileBinary;

NQ_EXPORT NQAssetSystemBinary* NQAssetSystemBinary_create(const uint8_t* data, size_t size);
NQ_EXPORT void NQAssetSystemBinary_destroy(NQAssetSystemBinary* system);

NQ_EXPORT NQAssetDirBinary* NQAssetBinary_openDir(NQAssetSystemBinary* system, const char* dirname);
NQ_EXPORT const char* NQAssetDirBinary_readFileName(NQAssetDirBinary* dir);
NQ_EXPORT void NQAssetDirBinary_close(NQAssetDirBinary* dir);

NQ_EXPORT NQAssetFileBinary* NQAssetBinary_openFile(NQAssetSystemBinary* system, const char* filename, int mode);
NQ_EXPORT int64_t NQAssetFileBinary_getSize(NQAssetFileBinary* file);
NQ_EXPORT int NQAssetFileBinary_read(NQAssetFileBinary* file, void* buffer, size_t size);
NQ_EXPORT void NQAssetFileBinary_close(NQAssetFileBinary* file);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ASSETBINARY_H */
