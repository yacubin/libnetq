/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/wasm/ModuleTypes.h"

const char* NQGetWasmSectionNameById(int sectionId)
{
  switch (sectionId) {
  case NQ_WASM_SECTION_CUSTOM_ID:
    return "Custom";
  case NQ_WASM_SECTION_TYPE_ID:
    return "Type";
  case NQ_WASM_SECTION_IMPORT_ID:
    return "Import";
  case NQ_WASM_SECTION_FUNCTION_ID:
    return "Function";
  case NQ_WASM_SECTION_TABLE_ID:
    return "Table";
  case NQ_WASM_SECTION_MEMORY_ID:
    return "Memory";
  case NQ_WASM_SECTION_GLOBAL_ID:
    return "Global";
  case NQ_WASM_SECTION_EXPORT_ID:
    return "Export";
  case NQ_WASM_SECTION_START_ID:
    return "Start";
  case NQ_WASM_SECTION_ELEMENT_ID:
    return "Element";
  case NQ_WASM_SECTION_CODE_ID:
    return "Code";
  case NQ_WASM_SECTION_DATA_ID:
    return "Data";
  case NQ_WASM_SECTION_DATA_COUNT_ID:
    return "DataCount";
  }

  return "Unknown";
}
