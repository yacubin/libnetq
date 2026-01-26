/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_FILESYSTEM_H
#define _LIBNETQ_FILESYSTEM_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT int64_t NQFileSystem_read(const char* path, uint8_t* data, int64_t size);
NQ_EXPORT int64_t NQFileSystem_write(const char* path, const uint8_t* data, int64_t size);
NQ_EXPORT bool NQFileSystem_mkdir(const char* path, bool recursive);
NQ_EXPORT bool NQFileSystem_exists(const char* path);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_FILESYSTEM_H */
