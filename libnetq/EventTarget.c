/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/EventTarget.h"

#include <libnetq/Malloc.h> // TODO: Remove allocation

struct NQEventTargetListener {
  struct NQEventTargetListener* next;
  void* userdata;
  NQEventCallback* callback;
};

void NQEventTarget_init(NQEventTarget* thiz)
{
  thiz->first = NULL;
}

void NQEventTarget_finalize(NQEventTarget* thiz)
{
  struct NQEventTargetListener* curr = thiz->first;
  while (curr != NULL) {
    struct NQEventTargetListener* next = curr->next;
    NQFree(curr);
    curr = next;
  }
}

void NQEventTarget_addEventListener(NQEventTarget* thiz, NQEventCallback* callback, void* userdata)
{
  if (callback == NULL)
    return;

  struct NQEventTargetListener* curr = (struct NQEventTargetListener*)NQMalloc(sizeof(struct NQEventTargetListener*));
  if (curr == NULL)
    return;

  curr->next = thiz->first;
  curr->userdata = userdata;
  curr->callback = callback;

  thiz->first = curr;
}

void NQEventTarget_removeEventListener(NQEventTarget* thiz, NQEventCallback* callback)
{
  struct NQEventTargetListener* prev = NULL;
  struct NQEventTargetListener* curr = thiz->first;
  while (curr != NULL) {
    if (curr->callback == callback) {
      if (prev == NULL)
        thiz->first = curr->next;
      else
        prev->next = curr->next;
      NQFree(curr);
      break;
    }
    prev = curr;
    curr = curr->next;
  }
}

bool NQEventTarget_dispatchEvent(NQEventTarget* thiz, const NQEvent* e)
{
  bool cancelBubble = false;

  struct NQEventTargetListener* curr = thiz->first;
  while (curr != NULL) {
    cancelBubble = (curr->callback)(curr->userdata, e);
    if (cancelBubble)
      break;
    curr = curr->next;
  }

  return cancelBubble;
}
