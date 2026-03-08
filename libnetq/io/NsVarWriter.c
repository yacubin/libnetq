/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/io/NsVarWriter.h"

#include <libnetq/String.h>
#include <libnetq/CType.h>
#include <libnetq/Math.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>
#include <libnetq/Malloc.h>

enum {
  kEnvInitState,
  kEnvPrefixState,
  kEnvKeyState,
  kEnvValueState,
  kEnvErrorState,
};

NQNsVarWriter* NQNsVarWriter_create(const struct NQNsVarWriterEntry* entries, NQWriteCallback writer, void* userdata)
{
  NQNsVarWriter* thiz = (NQNsVarWriter*)NQMalloc(sizeof(NQNsVarWriter));
  if (thiz)
    NQNsVarWriter_init(thiz, entries, writer, userdata);
  return thiz;
}

void NQNsVarWriter_release(NQNsVarWriter* thiz)
{
  NQNsVarWriter_finalize(thiz);
  NQFree(thiz);
}

void NQNsVarWriter_init(NQNsVarWriter* thiz, const struct NQNsVarWriterEntry* entries, NQWriteCallback writer, void* userdata)
{
  NQWriteWrapper_init(&thiz->base, writer, userdata);
  thiz->entries = entries;
  thiz->separator = ':';
  thiz->state = kEnvInitState;
  thiz->bufferSize = 0;
}

int NQNsVarWriter_write(NQNsVarWriter* thiz, const void* data, size_t size)
{
  int res = NQGetMin(NQ_INT32_MIN, size);

  const char* ptr = (const char*)data;
  const char* end = ptr + res;
  const char* start = (thiz->state == kEnvInitState) ? ptr : NULL;

  while (ptr < end) {
    char c = *ptr;

    switch (thiz->state) {
    case kEnvInitState:
      if (c == '$') {
        thiz->buffer[0] = c;
        thiz->bufferSize = 1;
        thiz->state = kEnvPrefixState;
      }
      ptr++;
      break;

    case kEnvPrefixState:
      if (c == '{') {
        thiz->buffer[thiz->bufferSize++] = c;
        thiz->state = kEnvKeyState;
        ptr++;
      }
      else if (start == NULL && !NQWriteWrapper_writeAll(&thiz->base, thiz->buffer, thiz->bufferSize)) {
        thiz->state = kEnvErrorState;
        return -1;
      }
      else {
        thiz->state = kEnvInitState;
        thiz->bufferSize = 0;
      }
      break;

    case kEnvKeyState:
      if (c == '$' || c == '{' || c == '}' || sizeof(thiz->buffer) <= thiz->bufferSize) {
        if (start == NULL && !NQWriteWrapper_writeAll(&thiz->base, thiz->buffer, thiz->bufferSize)) {
          thiz->state = kEnvErrorState;
          return -1;
        }
        thiz->state = kEnvInitState;
        thiz->bufferSize = 0;
        break;
      }

      if (c != thiz->separator) {
        thiz->buffer[thiz->bufferSize++] = c;
        ptr++;
        break;
      }

      thiz->buffer[thiz->bufferSize] = '\0';

      thiz->current = thiz->entries;
      while (true) {
        if (thiz->current->name == NULL) {
          thiz->current = NULL;
          break;
        }
        if (strcmp(thiz->current->name, thiz->buffer + 2) == 0)
          break;
        thiz->current++;
      }

      thiz->buffer[thiz->bufferSize++] = c;
      ptr++;

      if (thiz->current == NULL) {
        if (start == NULL && !NQWriteWrapper_writeAll(&thiz->base, thiz->buffer, thiz->bufferSize)) {
          thiz->state = kEnvErrorState;
          return -1;
        }
        thiz->state = kEnvInitState;
        thiz->bufferSize = 0;
        break;
      }

      thiz->valueIndex = thiz->bufferSize;
      thiz->state = kEnvValueState;
      break;

    case kEnvValueState:
      if (c == '$' || c == '{' || sizeof(thiz->buffer) <= thiz->bufferSize) {
        if (start == NULL && !NQWriteWrapper_writeAll(&thiz->base, thiz->buffer, thiz->bufferSize)) {
          thiz->state = kEnvErrorState;
          return -1;
        }
        thiz->state = kEnvInitState;
        thiz->bufferSize = 0;
        break;
      }

      if (c != '}') {
        thiz->buffer[thiz->bufferSize++] = c;
        ptr++;
        break;
      }

      if (start != NULL && !NQWriteWrapper_writeAll(&thiz->base, start, ptr - start - thiz->bufferSize)) {
        thiz->state = kEnvErrorState;
        return -1;
      }

      thiz->buffer[thiz->bufferSize] = '\0';
      ptr++;

      if (thiz->current->handler(thiz->buffer + thiz->valueIndex, thiz->base.write, thiz->base.userdata) < 0) {
        thiz->state = kEnvErrorState;
        return -1;
      }

      start = ptr;
      thiz->state = kEnvInitState;
      thiz->bufferSize = 0;
      break;

    case kEnvErrorState:
      return -1;

    default:
      NQ_ASSERT_NOT_REACHED();
      return -1;
    }
  }

  if (start != NULL && !NQWriteWrapper_writeAll(&thiz->base, start, end - start - thiz->bufferSize)) {
    thiz->state = kEnvErrorState;
    return -1;
  }

  return res;
}

int NQNsVarWriter_flush(NQNsVarWriter* thiz)
{
  switch (thiz->state) {
  case kEnvInitState:
    break;

  case kEnvPrefixState:
  case kEnvKeyState:
  case kEnvValueState:
    if (!NQWriteWrapper_writeAll(&thiz->base, thiz->buffer, thiz->bufferSize)) {
      thiz->state = kEnvErrorState;
      return -1;
    }
    thiz->state = kEnvInitState;
    thiz->bufferSize = 0;
    break;

  case kEnvErrorState:
    return -1;

  default:
    NQ_ASSERT_NOT_REACHED();
    return -1;
  }

  return 0;
}
