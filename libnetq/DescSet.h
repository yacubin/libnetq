/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DESCSET_H
#define _LIBNETQ_DESCSET_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_DESC_INVALID (0)
#define NQ_DESC_NUM_MAX (1 << 12)

#define NQDescIdentifierToIndex(id) ((id) & (NQ_DESC_NUM_MAX - 1))

typedef uintptr_t NQDescIdentifier;
typedef struct NQDescSet NQDescSet;
typedef struct NQDescEntry NQDescEntry;

NQ_EXPORT NQDescSet* NQDescSet_create(size_t num, size_t sizeInBytes);
NQ_EXPORT void NQDescSet_destroy(NQDescSet* descset);

NQ_EXPORT bool NQDescSet_setMaxId(NQDescSet* descset, NQDescIdentifier maxIdentifier);
NQ_EXPORT size_t NQDescSet_size(const NQDescSet* descset);
NQ_EXPORT bool NQDescSet_isEmpty(const NQDescSet* descset);

NQ_EXPORT NQDescIdentifier NQDescSet_allocEntry(NQDescSet* descset, void** ptr);
NQ_EXPORT void NQDescSet_releaseEntry(NQDescSet* descset, NQDescIdentifier id);
NQ_EXPORT void* NQDescSet_findData(NQDescSet* descset, NQDescIdentifier id);
NQ_EXPORT bool NQDescSet_hasEntry(NQDescSet* descset, NQDescIdentifier id);

NQ_EXPORT NQDescEntry* NQDescSet_first(NQDescSet* descset);
NQ_EXPORT NQDescEntry* NQDescEntry_next(NQDescEntry* entry);
NQ_EXPORT void* NQDescEntry_data(NQDescEntry* entry);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_DESCSET_H */
