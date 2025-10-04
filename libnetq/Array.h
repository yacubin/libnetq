/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_ARRAY_H
#define _LIBNETQ_ARRAY_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQUint8Array NQUint8Array;

NQ_EXPORT NQUint8Array* NQUint8Array_alloc(size_t size);
NQ_EXPORT NQUint8Array* NQUint8Array_create(const uint8_t* data, size_t size);
NQ_EXPORT void NQUint8Array_destroy(NQUint8Array* array);
NQ_EXPORT NQUint8Array* NQUint8Array_copy(const NQUint8Array* array);
NQ_EXPORT uint8_t* NQUint8Array_data(NQUint8Array* array);
NQ_EXPORT const uint8_t* NQUint8Array_cdata(const NQUint8Array* array);
NQ_EXPORT size_t NQUint8Array_size(const NQUint8Array* array);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_ARRAY_H */
