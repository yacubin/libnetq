/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_HEXVIEWER_H
#define _LIBNETQ_HEXVIEWER_H

#include <libnetq/Basic.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_HEXVIEWER_LOWERCASTE (1 << 0)
#define NQ_HEXVIEWER_UPPERCASTE 0

typedef struct NQHexViewer NQHexViewer;

NQHexViewer* NQHexViewer_create(int mode);
void NQHexViewer_destroy(NQHexViewer* viewer);
void NQHexViewer_reset(NQHexViewer* viewer);
void NQHexViewer_setOffset(NQHexViewer* viewer, size_t offset);
void NQHexViewer_addData(NQHexViewer* viewer, const void* data, size_t size);
const char* NQHexViewer_characters(const NQHexViewer* viewer);
size_t NQHexViewer_length(const NQHexViewer* viewer);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_HEXVIEWER_H */
