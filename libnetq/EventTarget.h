/*
 * MIT License
 *
 * Copyright (c) 2021-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_EVENTTARGET_H
#define _LIBNETQ_EVENTTARGET_H

#include <libnetq/Basic.h>
#include <libnetq/Event.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQEventTarget {
  struct NQEventTargetListener* first;
} NQEventTarget;

NQ_EXPORT void NQEventTarget_init(NQEventTarget*);
NQ_EXPORT void NQEventTarget_finalize(NQEventTarget*);

NQ_EXPORT void NQEventTarget_addEventListener(NQEventTarget*, NQEventCallback* callback, void* userdata);
NQ_EXPORT void NQEventTarget_removeEventListener(NQEventTarget*, NQEventCallback* callback);
NQ_EXPORT bool NQEventTarget_dispatchEvent(NQEventTarget*, const NQEvent* e);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_EVENTTARGET_H */
