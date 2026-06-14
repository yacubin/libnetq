/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_LOGCIRCULAR_H
#define _LIBNETQ_LOGCIRCULAR_H

#include <libnetq/VA.h>
#include <libnetq/Log.h>

#ifdef __cplusplus
extern "C" {
#endif
  
typedef struct NQLogCircular NQLogCircular;

NQ_EXPORT NQLogCircular* NQLogCircular_create(size_t capacity);
NQ_EXPORT void NQLogCircular_destroy(NQLogCircular* log);

NQ_EXPORT size_t NQLogCircular_print(NQLogCircular* log, NQLogLevel level, const char* format, ...) NQ_ATTRIBUTE_PRINTF(3, 4);
NQ_EXPORT size_t NQLogCircular_vprint(NQLogCircular* log, NQLogLevel level, const char* format, va_list args);

NQ_EXPORT size_t NQLogCircular_count(const NQLogCircular* log);
NQ_EXPORT bool NQLogCircular_isFull(const NQLogCircular* log);

NQ_EXPORT const char* NQLogCircular_charactersAt(const NQLogCircular* log, size_t index);
NQ_EXPORT size_t NQLogCircular_lengthAt(const NQLogCircular* log, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_LOGCIRCULAR_H */
