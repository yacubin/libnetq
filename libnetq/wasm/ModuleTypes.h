/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WASM_MODULETYPES_H
#define _LIBNETQ_WASM_MODULETYPES_H

#include <libnetq/Basic.h>

#define	NQ_WASM_MAG0 0x00
#define	NQ_WASM_MAG1 'a'
#define	NQ_WASM_MAG2 's'
#define	NQ_WASM_MAG3 'm'
#define	NQ_WASM_MAG_STR "\0asm"
#define NQ_WASM_MAG_LEN 4

#define NQ_WASM_MAGIC 0x6d736100
#define NQ_WASM_VERSION 1

enum {
  NQ_WASM_SECTION_CUSTOM_ID = 0,
  NQ_WASM_SECTION_TYPE_ID = 1,
  NQ_WASM_SECTION_IMPORT_ID = 2,
  NQ_WASM_SECTION_FUNCTION_ID = 3,
  NQ_WASM_SECTION_TABLE_ID = 4,
  NQ_WASM_SECTION_MEMORY_ID = 5,
  NQ_WASM_SECTION_GLOBAL_ID = 6,
  NQ_WASM_SECTION_EXPORT_ID = 7,
  NQ_WASM_SECTION_START_ID = 8,
  NQ_WASM_SECTION_ELEMENT_ID = 9,
  NQ_WASM_SECTION_CODE_ID = 10,
  NQ_WASM_SECTION_DATA_ID = 11,
  NQ_WASM_SECTION_DATA_COUNT_ID = 12,
};

NQ_EXPORT const char* NQGetWasmSectionNameById(int sectionId);

enum {
  NQ_WASM_IMPORT_FUNC_ID = 0,
  NQ_WASM_IMPORT_TABLE_ID = 1,
  NQ_WASM_IMPORT_MEM_ID = 2,
  NQ_WASM_IMPORT_GLOBAL_ID = 3,
};

struct NQWasmHeader {
  uint32_t magic;
  uint32_t version;
};

#define NQ_WASM_MEMTYPE_MAXVAL (1 << 0)
#define NQ_WASM_MEMTYPE_SHARED (1 << 1)

struct NQWasmMemoryInfo {
  uint32_t minValue;
  uint32_t maxValue;
  bool hasMaxValue;
  bool isShared;
};

#endif /* _LIBNETQ_WASM_MODULETYPES_H */
