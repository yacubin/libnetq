/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/wasm/Module.h"

#include <libnetq/BufferBuilder.h>
#include <libnetq/ByteBuffer.h>
#include <libnetq/Malloc.h>
#include <libnetq/String.h>
#include <libnetq/Log.h>
#include <libnetq/Leb128.h>
#include <libnetq/Math.h>
#include <libnetq/Limits.h>
#include <libnetq/Assert.h>
#include <libnetq/io/ReadWrapper.h>
#include <libnetq/io/WriteWrapper.h>
#include <libnetq/io/DataReader.h>

#if NQ_HAS_BUILTIN(__builtin_types_compatible_p)
NQ_STATIC_ASSERT(__builtin_types_compatible_p(NQWasmWriteCallback, NQWriteCallback), \
                 "NQWasmWriteCallback and NQWriteCallback are different types");
#endif

static bool readLeb128Uint32(NQDataReader* reader, uint32_t* value)
{
  uint8_t byte;
  NQLeb128Dec dec;
  NQLeb128Dec_init(&dec, false);

  bool done = false;
  do {
    if (!NQDataReader_readUint8(reader, &byte))
      return false;
    done = NQLeb128Dec_update(&dec, byte);
    if (sizeof(*value) < NQLeb128Dec_size(&dec))
      return false;
  } while (!done);

  *value = NQLeb128Dec_valueUint32(&dec);
  return true;
}

static bool readLeb128Uint64(NQDataReader* reader, uint64_t* value)
{
  uint8_t byte;
  NQLeb128Dec dec;
  NQLeb128Dec_init(&dec, false);

  bool done = false;
  do {
    if (!NQDataReader_readUint8(reader, &byte))
      return false;
    done = NQLeb128Dec_update(&dec, byte);
    if (sizeof(*value) < NQLeb128Dec_size(&dec))
      return false;
  } while (!done);

  *value = NQLeb128Dec_valueUint64(&dec);
  return true;
}

static bool readString(NQDataReader* thiz, NQStringParams* value)
{
  if (!readLeb128Uint32(thiz, &value->length))
    return false;

  value->characters = (char*)NQDataReader_currentData(thiz);
  return NQDataReader_skipAll(thiz, value->length);
}

typedef struct NQWasmUnknownSection NQWasmUnknownSection;
struct NQWasmUnknownSection {
  NQWasmSection base;
  uint32_t size;
  uint8_t data[1];
};

static bool writeLeb128Uint32(NQWriteWrapper* thiz, uint32_t value)
{
  NQLeb128Buffer buffer;
  size_t size = NQLeb128EncodeUint32(buffer, sizeof(buffer), value);
  return NQWriteWrapper_writeAll(thiz, buffer, size);
}

static bool writeLeb128Uint64(NQWriteWrapper* thiz, uint32_t value)
{
  NQLeb128Buffer buffer;
  size_t size = NQLeb128EncodeUint64(buffer, sizeof(buffer), value);
  return NQWriteWrapper_writeAll(thiz, buffer, size);
}

static bool writeString(NQWriteWrapper* thiz, const char* value)
{
  uint32_t length = (uint32_t)strlen(value);
  if (!writeLeb128Uint32(thiz, length))
    return false;
  return NQWriteWrapper_writeAll(thiz, value, length);
}

static bool writeWasmSection(NQWriteWrapper* thiz, uint8_t sectionId, const void* data, uint32_t size)
{
  if (!NQWriteWrapper_writeUint8(thiz, sectionId))
    return false;
  if (!writeLeb128Uint32(thiz, size))
    return false;
  return NQWriteWrapper_writeAll(thiz, data, size);
}

static NQWasmUnknownSection* wasmUnknownSectionCreate(uint8_t sectionId, const uint8_t* data, uint32_t size)
{
  NQWasmUnknownSection* thiz = (NQWasmUnknownSection*)NQMalloc(sizeof(NQWasmUnknownSection) - sizeof(thiz->data) + size);
  if (thiz == NULL)
    return NULL;

  thiz->base.sectionId = sectionId;
  NQListHead_init(&thiz->base.list);
  thiz->size = size;
  memcpy(thiz->data, data, size);
  return thiz;
}

static void wasmUnknownSectionDestroy(NQWasmUnknownSection* thiz)
{
  NQListHead_remove(&thiz->base.list);
  NQFree(thiz);
}

static inline bool wasmUnknownSectionWriteTo(NQWasmUnknownSection* thiz, NQByteBuffer* tmpBuf, NQWriteWrapper* writer)
{
  return writeWasmSection(writer, thiz->base.sectionId, thiz->data, thiz->size);
}

static int writeByteBuffer(void* userdata, const void* data, size_t size)
{
  NQByteBuffer* buffer = (NQByteBuffer*)userdata;
  int n = (int)NQGetMin(NQ_INT32_MAX, size);
  return NQByteBuffer_append(buffer, (const uint8_t*)data, n) ? n : -1;
}

static struct NQWasmImportItem* createImportItem(uint8_t importId, const NQStringParams* moduleName, const NQStringParams* itemName)
{
  struct NQWasmImportItem* thiz = (struct NQWasmImportItem*)NQMalloc(sizeof(*thiz) + moduleName->length + itemName->length + 2);
  if (thiz == NULL) {
    NQ_LOGE("No memory");
    return NULL;
  }

  thiz->importId = importId;
  NQListHead_init(&thiz->list);

  char* ptr = (char*)thiz + sizeof(*thiz);
  thiz->module = ptr;
  memcpy(ptr, moduleName->characters, moduleName->length);
  ptr += moduleName->length;
  *ptr++ = '\0';

  thiz->name = ptr;
  memcpy(ptr, itemName->characters, itemName->length);
  ptr[itemName->length] = '\0';

  return thiz;
}

static struct NQWasmImportItem* createImportFunction(const NQStringParams* moduleName, const NQStringParams* itemName, uint32_t typeidx)
{
  struct NQWasmImportItem* thiz = createImportItem(NQ_WASM_IMPORT_FUNC_ID, moduleName, itemName);
  if (thiz == NULL)
    return NULL;

  thiz->function.typeidx = typeidx;

  return thiz;
}

static struct NQWasmImportItem* readImportFunction(const NQStringParams* moduleName, const NQStringParams* itemName, NQDataReader* reader)
{
  uint32_t typeidx;
  if (!readLeb128Uint32(reader, &typeidx)) {
    NQ_LOGE("Unexpected end of typeidx reached");
    return NULL;
  }
  return createImportFunction(moduleName, itemName, typeidx);
}

static struct NQWasmImportItem* createImportTable(const NQStringParams* moduleName, const NQStringParams* itemName, uint8_t elemtype, uint32_t minValue, uint32_t maxValue)
{
  struct NQWasmImportItem* thiz = createImportItem(NQ_WASM_IMPORT_TABLE_ID, moduleName, itemName);
  if (thiz == NULL)
    return NULL;

  thiz->table.elemtype = elemtype;
  thiz->table.minValue = minValue;
  thiz->table.maxValue = maxValue;

  return thiz;
}

static struct NQWasmImportItem* readImportTable(const NQStringParams* moduleName, const NQStringParams* itemName, NQDataReader* reader)
{
  uint8_t elemtype;
  uint32_t minValue;
  uint32_t maxValue;

  if (!NQDataReader_readUint8(reader, &elemtype)) {
    NQ_LOGE("Unexpected end of elemtype reached");
    return NULL;
  }
  if (!NQWasmIsElemType(elemtype)) {
    NQ_LOGE("Unknown %02x elemtype of import global", elemtype);
    return NULL;
  }
  if (!readLeb128Uint32(reader, &minValue)) {
    NQ_LOGE("Minimum value of import table is wrong");
    return NULL;
  }
  if (!readLeb128Uint32(reader, &maxValue)) {
    NQ_LOGE("Maximum value of import table is wrong");
    return NULL;
  }

  return createImportTable(moduleName, itemName, elemtype, minValue, maxValue);
}

static struct NQWasmImportItem* createImportMemory(const NQStringParams* moduleName, const NQStringParams* itemName, uint8_t memtype, uint64_t minValue, uint64_t maxValue)
{
  struct NQWasmImportItem* thiz = createImportItem(NQ_WASM_IMPORT_MEM_ID, moduleName, itemName);
  if (thiz == NULL)
    return NULL;

  thiz->memory.memtype = memtype;
  thiz->memory.minValue = minValue;
  thiz->memory.maxValue = maxValue;

  return thiz;
}

static struct NQWasmImportItem* readImportMemory(const NQStringParams* moduleName, const NQStringParams* itemName, NQDataReader* reader)
{
  uint8_t memtype;
  uint64_t minValue;
  uint64_t maxValue;

  if (!NQDataReader_readUint8(reader, &memtype)) {
    NQ_LOGE("Unexpected end of memtype reached");
    return NULL;
  }
  if (!NQWasmIsMemType(memtype)) {
    NQ_LOGE("Unknown %02x memtype of import memory", memtype);
    return NULL;
  }
  if (!readLeb128Uint64(reader, &minValue)) {
    NQ_LOGE("Minimum value of import memory is wrong");
    return NULL;
  }
  if (memtype & NQ_WASM_MEMTYPE_MAXVAL) {
    if (!readLeb128Uint64(reader, &maxValue)) {
      NQ_LOGE("Maximum value of import memory is wrong");
      return NULL;
    }
  }
  else {
    maxValue = 0;
  }

  return createImportMemory(moduleName, itemName, memtype, minValue, maxValue);
}

static struct NQWasmImportItem* createImportGlobal(const NQStringParams* moduleName, const NQStringParams* itemName, uint8_t valtype, uint8_t mut)
{
  struct NQWasmImportItem* thiz = createImportItem(NQ_WASM_IMPORT_GLOBAL_ID, moduleName, itemName);
  if (thiz == NULL)
    return NULL;

  thiz->global.valtype = valtype;
  thiz->global.mut = mut;

  return thiz;
}

static struct NQWasmImportItem* readImportGlobal(const NQStringParams* moduleName, const NQStringParams* itemName, NQDataReader* reader)
{
  uint8_t valtype;
  uint8_t mut;

  if (!NQDataReader_readUint8(reader, &valtype)) {
    NQ_LOGE("Unexpected end of numtype reached");
    return NULL;
  }
  if (!NQWasmIsValType(valtype)) {
    NQ_LOGE("Unknown %02x numtype of import global", valtype);
    return NULL;
  }
  if (!NQDataReader_readUint8(reader, &mut)) {
    NQ_LOGE("Unexpected end of numtype reached");
    return NULL;
  }

  return createImportGlobal(moduleName, itemName, valtype, mut);
}

NQWasmImportSection* NQWasmImportSection_create(void)
{
  NQWasmImportSection* thiz = (NQWasmImportSection*)NQMalloc(sizeof(*thiz));
  if (thiz != NULL)
    NQWasmImportSection_init(thiz);
  return thiz;
}

static inline void clearImportItemList(NQListHead* itemList)
{
  NQListHead* iter = itemList->next;
  while (iter != itemList) {
    NQWasmImportItem* item = NQ_CONTAINER_OF(iter, NQWasmImportItem, list);
    iter = iter->next;
    NQListHead_remove(&item->list);
    NQFree(item);
  }
}

NQWasmImportSection* NQWasmImportSection_fromMemory(const void* data, size_t size)
{
  NQDataReader reader;
  NQDataReader_init(&reader, data, size);

  uint32_t itemCount;
  if (!readLeb128Uint32(&reader, &itemCount)) {
    NQ_LOGE("Import vector count format is wrong");
    return false;
  }

  NQListHead itemList;
  NQListHead_init(&itemList);

  size_t itemIndex = 0;
  while (itemIndex < itemCount) {
    struct NQWasmImportItem* item = NULL;

    NQStringParams moduleName;
    if (!readString(&reader, &moduleName)) {
      NQ_LOGE("Import module name is wrong");
      break;
    }

    NQStringParams itemName;
    if (!readString(&reader, &itemName)) {
      NQ_LOGE("Import item name is wrong");
      break;
    }

    uint8_t importId;
    if (!NQDataReader_readUint8(&reader, &importId)) {
      NQ_LOGE("Import desc format is wrong");
      break;
    }

    switch (importId) {
    case NQ_WASM_IMPORT_FUNC_ID:
      item = readImportFunction(&moduleName, &itemName, &reader);
      break;

    case NQ_WASM_IMPORT_TABLE_ID:
      item = readImportTable(&moduleName, &itemName, &reader);
      break;

    case NQ_WASM_IMPORT_MEM_ID:
      item = readImportMemory(&moduleName, &itemName, &reader);
      break;

    case NQ_WASM_IMPORT_GLOBAL_ID:
      item = readImportGlobal(&moduleName, &itemName, &reader);
      break;

    default:
      NQ_LOGE("Unknown import id - %u", importId);
      break;
    }

    if (item == NULL)
      break;

    NQListHead_addBack(&itemList, &item->list);
    itemIndex++;
  }

  NQWasmImportSection* thiz = NULL;
  if (itemIndex == itemCount) {
    if (!NQDataReader_isEmpty(&reader))
      NQ_LOGE("Not the entire import section was processed");
    else
      thiz = NQWasmImportSection_create();
    if (thiz != NULL) {
      NQListHead_swap(&thiz->itemList, &itemList);
      thiz->itemCount = itemCount;
    }
  }

  clearImportItemList(&itemList);
  return thiz;
}

void NQWasmImportSection_init(NQWasmImportSection* thiz)
{
  thiz->base.sectionId = NQ_WASM_SECTION_IMPORT_ID;
  NQListHead_init(&thiz->base.list);
  NQListHead_init(&thiz->itemList);
  thiz->itemCount = 0;
}

void NQWasmImportSection_finalize(NQWasmImportSection* thiz)
{
  clearImportItemList(&thiz->itemList);
  NQListHead_remove(&thiz->base.list);
}

void NQWasmImportSection_destroy(NQWasmImportSection* thiz)
{
  NQWasmImportSection_finalize(thiz);
  NQFree(thiz);
}

static bool writeImportSectionContent(const NQWasmImportSection* thiz, NQWriteWrapper* writer)
{
  if (!writeLeb128Uint32(writer, thiz->itemCount))
    return false;

  struct NQWasmImportItem* item = NQWasmImportSection_firstItem(thiz);
  while (item != NULL) {
    if (!writeString(writer, item->module))
      return false;

    if (!writeString(writer, item->name))
      return false;

    if (!NQWriteWrapper_writeUint8(writer, item->importId))
      return false;

    switch (item->importId) {
    case NQ_WASM_IMPORT_FUNC_ID:
      if (!writeLeb128Uint32(writer, item->function.typeidx))
        return false;
      break;

    case NQ_WASM_IMPORT_TABLE_ID:
      if (!NQWriteWrapper_writeUint8(writer, item->table.elemtype))
        return false;
      if (!writeLeb128Uint32(writer, item->table.minValue))
        return false;
      if (!writeLeb128Uint32(writer, item->table.maxValue))
        return false;
      break;

    case NQ_WASM_IMPORT_MEM_ID:
      if (!NQWriteWrapper_writeUint8(writer, item->memory.memtype))
        return false;
      if (!writeLeb128Uint64(writer, item->memory.minValue))
        return false;
      if (item->memory.memtype & NQ_WASM_MEMTYPE_MAXVAL) {
        if (!writeLeb128Uint64(writer, item->memory.maxValue))
          return false;
      }
      break;

    case NQ_WASM_IMPORT_GLOBAL_ID:
      if (!NQWriteWrapper_writeUint8(writer, item->global.valtype))
        return false;
      if (!NQWriteWrapper_writeUint8(writer, item->global.mut))
        return false;
      break;

    default:
      NQ_ASSERT_NOT_REACHED();
      return false;
    }

    item = NQWasmImportSection_nextItem(thiz, item);
  }

  return true;
}

static bool wasmImportSectionWriteTo(const NQWasmImportSection* thiz, NQByteBuffer* tmpBuf, NQWriteWrapper* writer)
{
  NQByteBuffer_resize(tmpBuf, 0);

  NQWriteWrapper tmpWriter;
  NQWriteWrapper_init(&tmpWriter, writeByteBuffer, tmpBuf);

  if (!writeImportSectionContent(thiz, &tmpWriter))
    return false;

  return writeWasmSection(writer, thiz->base.sectionId, NQByteBuffer_data(tmpBuf), NQByteBuffer_size(tmpBuf));
}

bool NQWasmImportSection_writeTo(const NQWasmImportSection* thiz, NQWasmWriteCallback write, void* userdata)
{
  NQByteBuffer buffer;
  NQByteBuffer_init(&buffer);

  NQWriteWrapper writer;
  NQWriteWrapper_init(&writer, write, userdata);

  bool res = wasmImportSectionWriteTo(thiz, &buffer, &writer);
  NQByteBuffer_finalize(&buffer);
  return res;
}

bool NQWasmImportSection_addMemory(NQWasmImportSection* thiz, const char* module, const char* name, uint8_t memtype, uint64_t minValue, uint64_t maxValue)
{
  NQStringParams moduleName;
  moduleName.characters = module;
  moduleName.length = strlen(module);

  NQStringParams itemName;
  itemName.characters = name;
  itemName.length = strlen(name);

  NQWasmImportItem* item = createImportMemory(&moduleName, &itemName, memtype, minValue, maxValue);
  if (item == NULL)
    return false;

  NQListHead_addBack(&thiz->itemList, &item->list);
  thiz->itemCount++;
  return thiz;
}

NQWasmSection* NQWasmSection_fromMemory(uint8_t sectionId, const void* data, size_t size)
{
  switch (sectionId) {
  case NQ_WASM_SECTION_CUSTOM_ID:
  case NQ_WASM_SECTION_TYPE_ID:
    break;
  case NQ_WASM_SECTION_IMPORT_ID:
    return (NQWasmSection*)NQWasmImportSection_fromMemory(data, size);
  case NQ_WASM_SECTION_FUNCTION_ID:
  case NQ_WASM_SECTION_TABLE_ID:
  case NQ_WASM_SECTION_MEMORY_ID:
  case NQ_WASM_SECTION_GLOBAL_ID:
  case NQ_WASM_SECTION_EXPORT_ID:
  case NQ_WASM_SECTION_START_ID:
  case NQ_WASM_SECTION_ELEMENT_ID:
  case NQ_WASM_SECTION_CODE_ID:
  case NQ_WASM_SECTION_DATA_ID:
  case NQ_WASM_SECTION_DATA_COUNT_ID:
    break;
  default:
    NQ_LOGW("Unknown section id %i", sectionId);
    break;
  }

  return (NQWasmSection*)wasmUnknownSectionCreate(sectionId, data, size);
}

void NQWasmSection_destroy(NQWasmSection* thiz)
{
  switch (thiz->sectionId) {
  case NQ_WASM_SECTION_CUSTOM_ID:
  case NQ_WASM_SECTION_TYPE_ID:
    break;
  case NQ_WASM_SECTION_IMPORT_ID:
    NQWasmImportSection_destroy((NQWasmImportSection*)thiz);
    return;
  case NQ_WASM_SECTION_FUNCTION_ID:
  case NQ_WASM_SECTION_TABLE_ID:
  case NQ_WASM_SECTION_MEMORY_ID:
  case NQ_WASM_SECTION_GLOBAL_ID:
  case NQ_WASM_SECTION_EXPORT_ID:
  case NQ_WASM_SECTION_START_ID:
  case NQ_WASM_SECTION_ELEMENT_ID:
  case NQ_WASM_SECTION_CODE_ID:
  case NQ_WASM_SECTION_DATA_ID:
  case NQ_WASM_SECTION_DATA_COUNT_ID:
    break;
  }

  wasmUnknownSectionDestroy((NQWasmUnknownSection*)thiz);
}

static bool wasmSectionWriteTo(NQWasmSection* thiz, NQByteBuffer* tmpBuf, NQWriteWrapper* writer)
{
  switch (thiz->sectionId) {
  case NQ_WASM_SECTION_CUSTOM_ID:
  case NQ_WASM_SECTION_TYPE_ID:
    break;
  case NQ_WASM_SECTION_IMPORT_ID:
    return wasmImportSectionWriteTo((NQWasmImportSection*)thiz, tmpBuf, writer);
  case NQ_WASM_SECTION_FUNCTION_ID:
  case NQ_WASM_SECTION_TABLE_ID:
  case NQ_WASM_SECTION_MEMORY_ID:
  case NQ_WASM_SECTION_GLOBAL_ID:
  case NQ_WASM_SECTION_EXPORT_ID:
  case NQ_WASM_SECTION_START_ID:
  case NQ_WASM_SECTION_ELEMENT_ID:
  case NQ_WASM_SECTION_CODE_ID:
  case NQ_WASM_SECTION_DATA_ID:
  case NQ_WASM_SECTION_DATA_COUNT_ID:
    break;
  }

  return wasmUnknownSectionWriteTo((NQWasmUnknownSection*)thiz, tmpBuf, writer);
}

bool NQWasmSection_writeTo(NQWasmSection* thiz, NQWasmWriteCallback write, void* userdata)
{
  NQByteBuffer buffer;
  NQByteBuffer_init(&buffer);

  NQWriteWrapper writer;
  NQWriteWrapper_init(&writer, write, userdata);

  bool res = wasmSectionWriteTo(thiz, &buffer, &writer);
  NQByteBuffer_finalize(&buffer);
  return res;
}

static inline void clearSectionList(NQListHead* sectionList)
{
  NQListHead* iter = sectionList->next;
  while (iter != sectionList) {
    NQWasmSection* section = NQ_CONTAINER_OF(iter, NQWasmSection, list);
    iter = iter->next;
    NQWasmSection_destroy(section);
  }
}

void NQWasmModule_finalize(NQWasmModule* thiz)
{
  clearSectionList(&thiz->sectionList);
}

void NQWasmModule_destroy(NQWasmModule* thiz)
{
  clearSectionList(&thiz->sectionList);
  NQFree(thiz);
}

NQWasmSection* NQWasmModule_findSection(const NQWasmModule* thiz, uint8_t sectionId)
{
  NQWasmSection* iter = NQWasmModule_firstSection(thiz);
  while (iter != NULL) {
    if (iter->sectionId == sectionId)
      break;
    iter = NQWasmModule_nextSection(thiz, iter);
  }
  return iter;
}

static bool wasmModuleWriteTo(const NQWasmModule* module, NQWriteWrapper* writer)
{
  if (!NQWriteWrapper_writeUint32LE(writer, module->header.magic)) {
    NQ_LOGE("Can not write magic");
    return false;
  }

  if (!NQWriteWrapper_writeUint32LE(writer, module->header.version)) {
    NQ_LOGE("Can not write version");
    return false;
  }

  NQByteBuffer buffer;
  NQByteBuffer_init(&buffer);;

  NQWasmSection* section = NQWasmModule_firstSection(module);
  while (section != NULL) {
    if (!wasmSectionWriteTo(section, &buffer, writer)) {
      NQ_LOGE("Can not write %u section", section->sectionId);
      break;
    }
    section = NQWasmModule_nextSection(module, section);
  }

  NQByteBuffer_finalize(&buffer);
  return section == NULL;
}

bool NQWasmModule_writeTo(const NQWasmModule* thiz, NQWasmWriteCallback write, void* userdata)
{
  NQWriteWrapper writer;
  NQWriteWrapper_init(&writer, write, userdata);
  return wasmModuleWriteTo(thiz, &writer);
}

NQWasmModule* NQWasmModule_fromMemory(const void* data, size_t size)
{
  NQDataReader reader;
  NQDataReader_init(&reader, data, size);

  uint32_t magic;
  uint32_t version;

  if (!NQDataReader_readUint32LE(&reader, &magic)) {
    NQ_LOGE("Not enough data to get magic");
    return NULL;
  }

  if (magic != NQ_WASM_MAGIC) {
    NQ_LOGE("Magic is not correct 0x%04X", magic);
    return NULL;
  }

  if (!NQDataReader_readUint32LE(&reader, &version)) {
    NQ_LOGE("Not enough data to get version");
    return NULL;
  }

  if (version != NQ_WASM_VERSION) {
    NQ_LOGE("Version is not correct 0x%04X", version);
    return NULL;
  }

  NQListHead sectionList;
  NQListHead_init(&sectionList);

  while (!NQDataReader_isEmpty(&reader)) {
    uint8_t sectionId;
    if (!NQDataReader_readUint8(&reader, &sectionId)) {
      NQ_LOGE("Not enough data to get section id");
      break;
    }

    uint32_t sectionSize;
    if (!readLeb128Uint32(&reader, &sectionSize)) {
      NQ_LOGE("Section size format is wrong");
      break;
    }

    uint8_t* sectionData = NQDataReader_currentData(&reader);
    if (!NQDataReader_skipAll(&reader, sectionSize)) {
      NQ_LOGE("Not enough data to get section content");
      break;
    }

    NQWasmSection* section = NQWasmSection_fromMemory(sectionId, sectionData, sectionSize);
    if (section == NULL) {
      break;
    }

    NQListHead_addBack(&sectionList, &section->list);
  }

  NQWasmModule* thiz;
  if (NQDataReader_isEmpty(&reader)) {
    thiz = (NQWasmModule*)NQMalloc(sizeof(*thiz));
    if (thiz != NULL) {
      thiz->header.magic = magic;
      thiz->header.version = version;
      NQListHead_init(&thiz->sectionList);
      NQListHead_swap(&thiz->sectionList, &sectionList);
    }
  }
  else {
    thiz = NULL;
  }

  clearSectionList(&sectionList);
  return thiz;
}
