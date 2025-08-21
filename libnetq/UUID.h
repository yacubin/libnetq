/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_UUID_H
#define _LIBNETQ_UUID_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif
  
/* Universally unique identifier */

#define NQ_UUID_SIZE 16
#define NQ_UUID_STRING_SIZE (NQ_UUID_SIZE * 2 + 4)
#define NQ_UUID_SEPARATOR '-'

typedef struct NQUUID {
  uint8_t data[NQ_UUID_SIZE];
} NQUUID;

typedef struct NQUUIDData {
  uint32_t timeLow;
  uint16_t timeMid;
  uint16_t timeHiAndVersion;
  uint16_t clockSeq;
  uint8_t node[6];
} NQUUIDData;

typedef char NQUUIDBuffer[NQ_UUID_STRING_SIZE + 1];

NQ_EXPORT void NQUUID_init(NQUUID* uuid, const uint8_t* data, size_t size);
NQ_EXPORT void NQUUID_initAsCanonical(NQUUID* uuid);
NQ_EXPORT void NQUUID_initAsNil(NQUUID* uuid);
NQ_EXPORT void NQUUID_initAsOmni(NQUUID* uuid);
NQ_EXPORT bool NQUUID_parse(NQUUID* uuid, const char* str);
NQ_EXPORT bool NQUUID_parseWithLength(NQUUID* uuid, const char* str, size_t len);

NQ_EXPORT bool NQUUID_isNil(const NQUUID* uuid);
NQ_EXPORT bool NQUUID_isOmni(const NQUUID* uuid);
NQ_EXPORT bool NQUUID_isEqual(const NQUUID* a, const NQUUID* b);
NQ_EXPORT int NQUUID_sprintfLower(const NQUUID* uuid, char* buffer, size_t size);
NQ_EXPORT int NQUUID_sprintfUpper(const NQUUID* uuid, char* buffer, size_t size);
NQ_EXPORT int NQUUID_sprintfURN(const NQUUID* uuid, char* buffer, size_t size);

#define NQUUID_version(uuid) (((uuid)->data[6] & 0xF0) >> 4)

NQ_EXPORT const NQUUID* NQGetNilUUID();
NQ_EXPORT const NQUUID* NQGetOmniUUID();
NQ_EXPORT const char* NQGetNilUUIDString();
NQ_EXPORT const char* NQGetOmniUUIDString();
NQ_EXPORT const char* NQGetURNNIDUUIDString();

NQ_EXPORT int NQGenerateUUIDStringLower(char* buffer, size_t size);
NQ_EXPORT int NQGenerateUUIDStringUpper(char* buffer, size_t size);
NQ_EXPORT bool NQIsUUIDString(const char* str);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_UUID_H */
