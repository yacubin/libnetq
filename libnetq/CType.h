/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CTYPE_H
#define _LIBNETQ_CTYPE_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQIsASCII(c) (!((c) & ~0x7F))
#define NQIsSpace(c) ((c) == ' ' || (c) == '\n' || (c) == '\t' || (c) == '\r')
#define NQIsDigit(c) ('0' <= (c) && (c) <= '9')
#define NQIsLower(c) ('a' <= (c) && (c) <= 'z')
#define NQIsUpper(c) ('A' <= (c) && (c) <= 'Z')
#define NQIsAlpha(c) (NQIsLower((c) | 0x20))
#define NQIsBinaryDigit(c) ('0' == (c) || (c) == '1')
#define NQIsOctalDigit(c) ('0' <= (c) && (c) <= '7')
#define NQIsHexDigit(c) (NQIsDigit(c) || ('a' <= (c) && (c) <= 'f') || ('A' <= (c) && (c) <= 'F'))
#define NQIsPrintable(c) (' ' <= (c) && (c) <= '~')
#define NQToDigitValue(c) ((c) - '0')
#define NQToHexValue(c) ((c) < 'A' ? (c) - '0' : ((c) - 'A' + 10) & 0xF)

#define NQ_FALSE_STRING "false"
#define NQ_TRUE_STRING "true"
#define NQ_OFF_STRING "off"
#define NQ_ON_STRING "on"
#define NQ_ZERO_STRING "0"
#define NQ_ONE_STRING "1"

NQ_EXPORT bool NQIsFalse(const char* s);
NQ_EXPORT bool NQIsTrue(const char* s);
NQ_EXPORT bool NQIsOff(const char* s);
NQ_EXPORT bool NQIsOn(const char* s);
NQ_EXPORT bool NQIsEnabled(const char* s);
NQ_EXPORT bool NQIsDisabled(const char* s);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_CTYPE_H */
