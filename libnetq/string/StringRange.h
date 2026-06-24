/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_STRING_STRINGRANGE_H
#define _LIBNETQ_STRING_STRINGRANGE_H

#include <libnetq/string/StringUtil.h>

typedef struct NQStringRange NQStringRange;
struct NQStringRange {
  const char* characters;
  uint32_t length;
};

static inline bool NQStringRangeIsEqual(const NQStringRange* range, const char* str)
{
  return NQCStrIsEqual(str, range->characters, range->length);
}

#endif /* _LIBNETQ_STRING_STRINGRANGE_H */
