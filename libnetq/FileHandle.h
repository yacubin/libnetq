/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_FILEHANDLE_H
#define _LIBNETQ_FILEHANDLE_H

#include <libnetq/Basic.h>

#ifdef NQ_SYS_LINUX
#include <linux/err.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NQ_SYS_LINUX
typedef struct file* NQFileHandle;
#define NQIsFileValid(handle) (!IS_ERR(handle))
#define NQIsFileInvalid(handle) IS_ERR(handle)
#endif

#ifdef NQ_OS_WINDOWS
typedef void* HANDLE;
typedef HANDLE NQFileHandle;
#define NQ_INVALID_FILE ((HANDLE)-1)
#endif

#ifdef NQ_OS_UNIX
typedef int NQFileHandle;
#define NQ_INVALID_FILE (-1)
#endif

#if defined(NQ_OS_WINDOWS) || defined(NQ_OS_UNIX)
#define NQIsFileValid(handle) ((handle) != NQ_INVALID_FILE)
#define NQIsFileInvalid(handle) ((handle) == NQ_INVALID_FILE)
#endif

#define NQFileCheck(handle) NQIsFileValid(handle)

typedef enum NQFileOpenMode {
  NQ_FOPEN_READ = 0,
  NQ_FOPEN_WRITE
} NQFileOpenMode;

typedef enum NQFileSeekOrigin {
  NQ_FSEEK_SET = 0,
  NQ_FSEEK_CUR,
  NQ_FSEEK_END
} NQFileSeekOrigin;

NQ_EXPORT NQFileHandle NQFileOpen(const char* path, NQFileOpenMode mode);
NQ_EXPORT void NQFileClose(NQFileHandle handle);
NQ_EXPORT int NQFileRead(NQFileHandle handle, uint8_t* data, size_t size);
NQ_EXPORT int NQFileWrite(NQFileHandle handle, const uint8_t* data, size_t size);
NQ_EXPORT long long NQFileSeek(NQFileHandle handle, long long offset, NQFileSeekOrigin origin);
NQ_EXPORT long long NQFileGetSize(NQFileHandle handle);

NQ_EXPORT int64_t NQFileReadn(NQFileHandle handle, uint8_t* data, int64_t size);
NQ_EXPORT int64_t NQFileWriten(NQFileHandle handle, const uint8_t* data, int64_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_FILEHANDLE_H */
