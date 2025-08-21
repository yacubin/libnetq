/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRINGPRINT_H
#define _LIBNETQ_STRINGPRINT_H

#include <libnetq/Basic.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQStringPrint {
  char* characters;
  size_t length;
  size_t capacity;
  char buffer[128];
} NQStringPrint;

NQ_EXPORT void NQStringPrint_init(NQStringPrint*);
NQ_EXPORT void NQStringPrint_finalize(NQStringPrint*);

NQ_EXPORT void NQStringPrint_printf(NQStringPrint*, const char* format, ...) NQ_ATTRIBUTE_PRINTF(2, 3);
NQ_EXPORT int NQStringPrint_vprintf(NQStringPrint*, const char* format, va_list);
NQ_EXPORT int NQStringPrint_write(NQStringPrint*, const char* characters, size_t length);
NQ_EXPORT void NQStringPrint_reset(NQStringPrint*);

#define NQStringPrint_characters(thiz) (thiz)->characters
#define NQStringPrint_length(thiz) (thiz)->length

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_STRINGPRINT_H */
