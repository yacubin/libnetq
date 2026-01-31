/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_REGEX_REGEXPCRE2_H
#define _LIBNETQ_REGEX_REGEXPCRE2_H

#include <libnetq/Basic.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef pcre2_code NQRegex;

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_REGEX_REGEXPCRE2_H */
