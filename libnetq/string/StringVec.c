/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/string/StringVec.h"

#include <libnetq/string/String.h>
#include <libnetq/string/CStrBase.h>
#include <libnetq/Limits.h>
#include <libnetq/Malloc.h>
#include <libnetq/Log.h>

void NQStringVec_init(NQStringVec* thiz)
{
  thiz->characters = NQCStrEmpty();
  thiz->length = 0;
}

void NQStringVec_finalize(NQStringVec* thiz)
{
  if (thiz->characters != NQCStrEmpty())
    NQFree((void*)thiz->characters);
}

bool NQStringVec_setCharacters(NQStringVec* thiz, const char* characters)
{
  return NQStringVec_setCharacters2(thiz, characters, NQStrlen(characters));
}

bool NQStringVec_setCharacters2(NQStringVec* thiz, const char* characters, size_t length)
{
  if (length == 0) {
    NQStringVec_finalize(thiz);
    NQStringVec_init(thiz);
    return true;
  }

  if (length >= NQ_UINT32_MAX) {
    NQ_LOGE("String length exceeded");
    return false;
  }

  char* newCharacters = NQMalloc(length + 1);
  if (newCharacters == NULL) {
    return false;
  }

  memcpy(newCharacters, characters, length);
  newCharacters[length] = '\0';

  NQStringVec_finalize(thiz);

  thiz->characters = newCharacters;
  thiz->length = length;

  return true;
}

bool NQStringVec_isEqual(const NQStringVec* thiz, const char* str)
{
  size_t len = NQStrlen(str);
  return thiz->length == len && memcmp(thiz->characters, str, len) == 0;
}
