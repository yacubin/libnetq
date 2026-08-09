/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CONTEXT_H
#define _LIBNETQ_CONTEXT_H

#include <libnetq/Basic.h>
#include <libnetq/Notifier.h>
#include <libnetq/VA.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQContext NQContext;

NQ_EXPORT NQContext* NQContext_instance(void);

NQ_EXPORT int NQContext_loadModule(NQContext*, const char* format, ...) NQ_ATTRIBUTE_PRINTF(2, 3);

NQ_EXPORT void NQContext_notifyAll(NQContext*, unsigned event, void* data);
NQ_EXPORT void NQContext_registerNotifier(NQContext*, NQWebNotifierEntry* notifier);
NQ_EXPORT void NQContext_unregisterNotifier(NQContext*, NQWebNotifierEntry* notifier);
NQ_EXPORT void NQContext_unregisterAllNotifiers(NQContext*);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_PATH_H */
