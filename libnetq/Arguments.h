/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ARGUMENTS_H
#define _LIBNETQ_ARGUMENTS_H

#include <libnetq/Basic.h>

#ifdef NQ_OS_WINDOWS
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQArgDescriptor {
  const char* sname;
  const char* lname;
  const char* atype; // "" - no-arg, b - bool, u - unsigner, i - intager, r - real, s - string
  const char* desc;
} NQArgDescriptor;

typedef struct NQArgs {
  int argc;
  const char** argv;
} NQArgs;

NQ_EXPORT size_t NQArgs_count(NQArgs*);
NQ_EXPORT const char** NQArgs_items(NQArgs*);
NQ_EXPORT const char* NQArgs_at(NQArgs*, size_t index);

NQ_EXPORT bool NQArgs_has(NQArgs*, const char* value);
NQ_EXPORT bool NQArgs_hasLKey(NQArgs*, const char* value);

typedef struct NQArguments NQArguments;

NQ_EXPORT NQArguments* NQArguments_create(int argc, const char* argv[]);
NQ_EXPORT NQArguments* NQArguments_parse(const char*);
NQ_EXPORT NQArguments* NQArguments_parseUTF16(const uint16_t*);

NQ_EXPORT void NQArguments_destroy(NQArguments*);

NQ_EXPORT size_t NQArguments_count(NQArguments*);
NQ_EXPORT const char** NQArguments_items(NQArguments*);
NQ_EXPORT const char* NQArguments_at(NQArguments*, size_t index);

NQ_EXPORT bool NQArguments_has(NQArguments*, const char* value);
NQ_EXPORT bool NQArguments_hasLKey(NQArguments*, const char* value);

NQ_EXPORT void NQMainArgumentsInit(int argc, const char* argv[]);

#ifdef NQ_OS_WINDOWS

NQ_EXPORT void NQMainArgumentsInitWin(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT iCmdShow);
NQ_EXPORT void NQMainArgumentsInitWinW(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, INT iCmdShow);

NQ_EXPORT HINSTANCE NQGetInstance();
NQ_EXPORT HINSTANCE NQGetPrevInstance();
NQ_EXPORT INT NQGetCmdShow();

#endif /* NQ_OS_WINDOWS */

NQ_EXPORT void NQMainArgumentsFinalize();

NQArguments* NQGetMainArguments();

NQ_EXPORT const char* NQArgGetStringOr(const char* arg, const char* key, const char* defval);
NQ_EXPORT bool NQArgGetBoolOr(const char* arg, const char* key, bool defval);
NQ_EXPORT int32_t NQArgGetInt32Or(const char* arg, const char* key, int32_t defval);
NQ_EXPORT int64_t NQArgGetInt64Or(const char* arg, const char* key, int64_t defval);
NQ_EXPORT uint32_t NQArgGetUint32Or(const char* arg, const char* key, uint32_t defval);
NQ_EXPORT uint64_t NQArgGetUint64Or(const char* arg, const char* key, uint64_t defval);

NQ_EXPORT bool NQArgHasKey(const char* arg, const char* key);
NQ_EXPORT bool NQArgHasKeyOnly(const char* arg, const char* key);

NQ_EXPORT bool NQArgGetBool(const char* arg, const char* key, bool* result);
NQ_EXPORT bool NQArgGetInt32(const char* arg, const char* key, int32_t* result);
NQ_EXPORT bool NQArgGetInt64(const char* arg, const char* key, int64_t* result);
NQ_EXPORT bool NQArgGetUint32(const char* arg, const char* key, uint32_t* result);
NQ_EXPORT bool NQArgGetUint64(const char* arg, const char* key, uint64_t* result);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ARGUMENTS_H */
