/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#define NQ_CLASS_NAME "NQRegex"
#define NQ_LOG_TAG NQ_CLASS_NAME

#include "config.h"
#include "libnetq/regex/Regex.h"

#ifdef NQCONFIG_USE_STUB_REGEX

const char* NQRegex_package(void)
{
  return NULL;
}

const char* NQRegex_version(void)
{
  return NULL;
}

NQRegex* NQRegex_create(const char* pattern, int flags)
{
  NQ_UNUSED_PARAM(pattern);
  NQ_UNUSED_PARAM(flags);

  return NULL;
}

NQRegex* NQRegex_create2(const char* pattern, size_t length, int flags)
{
  NQ_UNUSED_PARAM(pattern);
  NQ_UNUSED_PARAM(length);
  NQ_UNUSED_PARAM(flags);

  return NULL;
}

void NQRegex_release(NQRegex* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

NQRegexMatch* NQRegexMatch_create(NQRegex* regex)
{
  NQ_UNUSED_PARAM(regex);

  return NULL;
}

NQRegexMatch* NQRegexMatch_create2(size_t size)
{
  NQ_UNUSED_PARAM(size);

  return NULL;
}

void NQRegexMatch_release(NQRegexMatch* thiz)
{
  NQ_UNUSED_PARAM(thiz);
}

size_t NQRegexMatch_count(const NQRegexMatch* thiz)
{
  NQ_UNUSED_PARAM(thiz);
  return 0;
}

bool NQRegexMatch_substrAt(const NQRegexMatch* thiz, size_t index, size_t* offset, size_t* length)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(index);
  NQ_UNUSED_PARAM(offset);
  NQ_UNUSED_PARAM(length);

  return false;
}

bool NQRegex_exec(NQRegex* thiz, const char* str, NQRegexMatch* match, int flags)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(str);
  NQ_UNUSED_PARAM(match);
  NQ_UNUSED_PARAM(flags);

  return false;
}

bool NQRegex_exec2(NQRegex* thiz, const char* str, size_t len, NQRegexMatch* match, int flags)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(str);
  NQ_UNUSED_PARAM(len);
  NQ_UNUSED_PARAM(match);
  NQ_UNUSED_PARAM(flags);

  return false;
}

int NQRegex_indexOf(const NQRegex* thiz, const char* name)
{
  NQ_UNUSED_PARAM(thiz);
  NQ_UNUSED_PARAM(name);

  return -1;
}

#endif
