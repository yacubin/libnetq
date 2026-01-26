/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_WASM_MODULE_H
#define _LIBNETQ_WASM_MODULE_H

#include <libnetq/List.h>
#include <libnetq/wasm/ModuleTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*NQWasmWriteCallback) (void* userdata, const void* data, size_t size);

typedef struct NQWasmSection NQWasmSection;
struct NQWasmSection {
  NQListHead list;
  uint8_t sectionId;
};

NQ_EXPORT NQWasmSection* NQWasmSection_fromMemory(uint8_t sectionId, const void* data, size_t size);
NQ_EXPORT void NQWasmSection_destroy(NQWasmSection*);
NQ_EXPORT bool NQWasmSection_writeTo(NQWasmSection*, NQWasmWriteCallback write, void* userdata);

typedef struct NQWasmImportItem NQWasmImportItem;
struct NQWasmImportItem {
  NQListHead list;
  uint8_t importId;

  const char* module;
  const char* name;

  union {
    struct {
      uint32_t typeidx;
    } function;

    struct {
      uint8_t elemtype;
      uint32_t minValue;
      uint32_t maxValue;
    } table;

    struct {
      uint8_t memtype;
      uint64_t minValue;
      uint64_t maxValue;
    } memory;

    struct {
      uint8_t valtype;
      uint8_t mut;
    } global;
  };
};

typedef struct NQWasmImportSection NQWasmImportSection;
struct NQWasmImportSection {
  NQWasmSection base;

  NQListHead itemList;
  uint32_t itemCount;
};

NQ_EXPORT NQWasmImportSection* NQWasmImportSection_create(void);
NQ_EXPORT NQWasmImportSection* NQWasmImportSection_fromMemory(const void* data, size_t size);
NQ_EXPORT void NQWasmImportSection_init(NQWasmImportSection*);
NQ_EXPORT void NQWasmImportSection_finalize(NQWasmImportSection*);
NQ_EXPORT void NQWasmImportSection_destroy(NQWasmImportSection*);
NQ_EXPORT bool NQWasmImportSection_writeTo(const NQWasmImportSection*, NQWasmWriteCallback write, void* userdata);
NQ_EXPORT bool NQWasmImportSection_addMemory(NQWasmImportSection*, const char* module, const char* name, uint8_t memtype, uint64_t minValue, uint64_t maxValue);

#define NQWasmImportSection_firstItem(thiz) ((thiz)->itemList.next != &(thiz)->itemList) \
  ? NQ_CONTAINER_OF((thiz)->itemList.next, NQWasmImportItem, list) : NULL
#define NQWasmImportSection_nextItem(thiz, item) ((item)->list.next != &(thiz)->itemList) \
  ? NQ_CONTAINER_OF((item)->list.next, NQWasmImportItem, list) : NULL

typedef struct NQWasmModule NQWasmModule; // ? WasmBinaryModule
struct NQWasmModule {
  struct NQWasmHeader header;
  NQListHead sectionList;
};

NQ_EXPORT void NQWasmModule_init(NQWasmModule*);
NQ_EXPORT NQWasmModule* NQWasmModule_fromMemory(const void* data, size_t size);
NQ_EXPORT void NQWasmModule_finalize(NQWasmModule*);
NQ_EXPORT void NQWasmModule_destroy(NQWasmModule*);
NQ_EXPORT bool NQWasmModule_writeTo(const NQWasmModule*, NQWasmWriteCallback write, void* userdata);

#define NQWasmModule_firstSection(thiz) (thiz)->sectionList.next != &(thiz)->sectionList \
  ? NQ_CONTAINER_OF((thiz)->sectionList.next, NQWasmSection, list) : NULL
#define NQWasmModule_nextSection(thiz, section) ((section)->list.next != &(thiz)->sectionList) \
  ? NQ_CONTAINER_OF((section)->list.next, NQWasmSection, list) : NULL

NQ_EXPORT NQWasmSection* NQWasmModule_findSection(const NQWasmModule*, uint8_t sectionId);

static inline NQWasmImportSection* NQWasmModule_findImportSection(const NQWasmModule* thiz)
{
  return (NQWasmImportSection*)NQWasmModule_findSection(thiz, NQ_WASM_SECTION_IMPORT_ID);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_WASM_MODULE_H */
