/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_FILEHANDLE_H
#define _LIBNETQ_FILEHANDLE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
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

#define NQFileCheck(handle) ((handle) != NQ_INVALID_FILE)
#define NQFileIsValid(handle) ((handle) != NQ_INVALID_FILE)
#define NQFileIsInvalid(handle) ((handle) == NQ_INVALID_FILE)

typedef enum NQFileOpenMode {
  NQ_FOPEN_READ = 0,
  NQ_FOPEN_WRITE
} NQFileOpenMode;

typedef enum NQFileSeekOrigin {
  NQ_FSEEK_SET = 0,
  NQ_FSEEK_CUR,
  NQ_FSEEK_END
} NQFileSeekOrigin;

NQFileHandle NQFileOpen(const char* path, NQFileOpenMode mode);
void NQFileClose(NQFileHandle handle);
int NQFileRead(NQFileHandle handle, uint8_t* data, size_t size);
int NQFileWrite(NQFileHandle handle, const uint8_t* data, size_t size);
long long NQFileSeek(NQFileHandle handle, long long offset, NQFileSeekOrigin origin);
long long NQFileGetSize(NQFileHandle handle);

int64_t NQFileReadn(NQFileHandle handle, uint8_t* data, int64_t size);
int64_t NQFileWriten(NQFileHandle handle, const uint8_t* data, int64_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_FILEHANDLE_H */
