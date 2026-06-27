/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_VERSION_H
#define _LIBNETQ_VERSION_H

#include <libnetq/Basic.h>
#include <libnetq/GenVersion.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t NQVersionCode;

typedef struct NQVersion {
  uint32_t data[4];
} NQVersion;

NQ_EXPORT void NQVersion_init(NQVersion* version);
NQ_EXPORT void NQVersion_init1(NQVersion* version, uint32_t major);
NQ_EXPORT void NQVersion_init2(NQVersion* version, uint32_t major, uint32_t minor);
NQ_EXPORT void NQVersion_init3(NQVersion* version, uint32_t major, uint32_t minor, uint32_t patch);
NQ_EXPORT void NQVersion_init4(NQVersion* version, uint32_t major, uint32_t minor, uint32_t patch, uint32_t tweak);

NQ_EXPORT uint32_t NQVersion_major(const NQVersion* version);
NQ_EXPORT void NQVersion_setMajor(NQVersion* version, uint32_t major);

NQ_EXPORT uint32_t NQVersion_minor(const NQVersion* version);
NQ_EXPORT void NQVersion_setMinor(NQVersion* version, uint32_t minor);

NQ_EXPORT uint32_t NQVersion_patch(const NQVersion* version);
NQ_EXPORT void NQVersion_setPatch(NQVersion* version, uint32_t patch);

NQ_EXPORT uint32_t NQVersion_tweak(const NQVersion* version);
NQ_EXPORT void NQVersion_setTweak(NQVersion* version, uint32_t tweak);

NQ_EXPORT bool NQVersion_isZero(const NQVersion* version);

NQ_EXPORT bool NQVersion_less(const NQVersion* a, const NQVersion* b);
NQ_EXPORT bool NQVersion_greater(const NQVersion* a, const NQVersion* b);
NQ_EXPORT bool NQVersion_equal(const NQVersion* a, const NQVersion* b);
NQ_EXPORT bool NQVersion_lessEqual(const NQVersion* a, const NQVersion* b);
NQ_EXPORT bool NQVersion_greaterEqual(const NQVersion* a, const NQVersion* b);

// TODO: NQVersion_parse
// TODO: NQVersion_sprintf

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_VERSION_H */
