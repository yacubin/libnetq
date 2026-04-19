/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/string/StringStorage.h"

#include <libnetq/CStrBase.h>
#include <libnetq/Limits.h>
#include <libnetq/Malloc.h>
#include <libnetq/Log.h>

void NQStringStorage_init(NQStringStorage* thiz)
{
  thiz->characters = NQCStrEmpty();
  thiz->length = 0;
}

void NQStringStorage_finalize(NQStringStorage* thiz)
{
  if (thiz->characters != NQCStrEmpty())
    NQFree((void*)thiz->characters);
}

bool NQStringStorage_setCharacters(NQStringStorage* thiz, const char* characters)
{
  return NQStringStorage_setCharacters2(thiz, characters, strlen(characters));
}

bool NQStringStorage_setCharacters2(NQStringStorage* thiz, const char* characters, size_t length)
{
  if (length == 0) {
    NQStringStorage_finalize(thiz);
    NQStringStorage_init(thiz);
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

  NQStringStorage_finalize(thiz);

  thiz->characters = newCharacters;
  thiz->length = length;

  return true;
}
