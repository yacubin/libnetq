/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCPARSER_H
#define _LIBNETQ_DBC_DBCPARSER_H

#include <libnetq/dbc/DBCTypes.h>
#include <libnetq/dbc/DBCErrorCode.h>
#include <libnetq/dbc/DBCAlloc.h>

#ifdef __cplusplus
extern "C" {
#endif

enum NQDBCSectionType {
  kNQDBCSectionVersion,
  kNQDBCSectionNewSymbols,
  kNQDBCSectionBitTiming,
  kNQDBCSectionNetNodes,
  kNQDBCSectionValTable,
  kNQDBCSectionMessage,
  kNQDBCSectionMessageTransmitter,
  kNQDBCSectionSignal,
  kNQDBCSectionSignalType,
  kNQDBCSectionSignalTypeRef,
  kNQDBCSectionSignalGroup,
  kNQDBCSectionSignalValue,
  kNQDBCSectionSignalValueType,
  kNQDBCSectionSignalMultiplexed,
  kNQDBCSectionEnvVar,
  kNQDBCSectionEnvVarData,
  kNQDBCSectionEnvVarValue,
  kNQDBCSectionComment,
  kNQDBCSectionAttr,
  kNQDBCSectionAttrRel,
  kNQDBCSectionAttrDefinition,
  kNQDBCSectionAttrRelDefinition,
  kNQDBCSectionAttrDefault,
  kNQDBCSectionAttrRelDefault,
};

struct NQDBCSectionVersion {
  int type;
  const char* data;
};

struct NQDBCSectionNewSymbols {
  int type;
  const char** data;
  size_t count;
};

struct NQDBCSectionBitTiming {
  int type;
  NQDBCBitTiming value;
};

struct NQDBCSectionNetNodes {
  int type;
  const char** names;
  size_t count;
};

struct NQDBCSectionValTable {
  int type;
  const char* name;
  const NQDBCValDesc* valDesc;
  size_t count;
};

struct NQDBCSectionMessage {
  int type;
  uint32_t id;
  uint32_t sizeInBytes;
  const char* name;
  const char* transmitter;
};

struct NQDBCSectionSignal {
  int type;

  const char* name;

  bool isMultiplexorSwitch;  // "M"
  bool hasMultiplexorValue;  // "m"
  uint32_t multiplexorValue;

  uint32_t startBit;

  NQDBCSigInfo info;

  const char** receivers;
  size_t count;
};

struct NQDBCSectionSignalType {
  int type;

  const char* name;
  NQDBCSigInfo info;
  double defaultValue;
  const char* valTable;
};

struct NQDBCSectionSignalTypeRef {
  int type;
  uint32_t messageId;
  const char* signalTypeName;
  const char* signalName;
};

struct NQDBCSectionSignalGroup {
  int type;
  uint32_t messageId;
  uint32_t repetitions;
  const char* name;
  const char** signals;
  size_t count;
};

struct NQDBCSectionMessageTransmitter {
  int type;
  uint32_t messageId;
  const char** transmitters;
  size_t count;
};

struct NQDBCSectionSignalValue {
  int type;
  uint32_t messageId;
  const char* signalName;
  const NQDBCValDesc* valDesc;
  size_t count;
};

struct NQDBCSectionSignalValueType {
  int type;
  NQDBCSigValueType value;
  uint32_t messageId;
  const char* signalName;
};

struct NQDBCSectionEnvVar {
  int type;
  const char* name;
  NQDBCEnvInfo info;
  const char** accessNode;
  size_t count;
};

struct NQDBCSectionEnvVarData {
  int type;
  uint32_t dataSize;
  const char* name;
};

struct NQDBCSectionEnvVarValue {
  int type;
  const char* name;
  const NQDBCValDesc* valDesc;
  size_t count;
};

struct NQDBCSectionComment {
  int type;
  NQDBCTarget target;
  const char* value;
};

struct NQDBCSectionAttrDefinition {
  int type;
  NQDBCObjectType objectType;
  struct NQDBCAttrValueInfo value;
  const char* name;
};

struct NQDBCSectionAttrDefault {
  int type;
  const char* name;
  NQDBCVariant value;
};

struct NQDBCSectionAttr {
  int type;
  const char* name;
  NQDBCTarget target;
  NQDBCVariant value;
};

struct NQDBCSectionAttrRel {
  int type;
  const char* name;
  const char* netNode;
  NQDBCTarget target;
  NQDBCVariant value;
};

struct NQDBCSectionSignalMultiplexed {
  int type;
  uint32_t messageId;
  const char* signalName;
  const char* switchName;
  const NQDBCRange* range;
  size_t count;
};

typedef union NQDBCSection NQDBCSection;
union NQDBCSection {
  enum NQDBCSectionType type;
  struct NQDBCSectionVersion version;
  struct NQDBCSectionNewSymbols newSymbols;
  struct NQDBCSectionBitTiming bitTiming;
  struct NQDBCSectionNetNodes netNodes;
  struct NQDBCSectionValTable valTable;
  struct NQDBCSectionMessage message;
  struct NQDBCSectionMessageTransmitter messageTransmitter;
  struct NQDBCSectionSignal signal;
  struct NQDBCSectionSignalType signalType;
  struct NQDBCSectionSignalTypeRef signalTypeRef;
  struct NQDBCSectionSignalGroup signalGroup;
  struct NQDBCSectionSignalValue signalValue;
  struct NQDBCSectionSignalValueType signalValueType;
  struct NQDBCSectionSignalMultiplexed signalMultiplexed;
  struct NQDBCSectionEnvVar envVar;
  struct NQDBCSectionEnvVarData envVarData;
  struct NQDBCSectionEnvVarValue envVarValue;
  struct NQDBCSectionComment comment;
  struct NQDBCSectionAttr attr;
  struct NQDBCSectionAttrRel attrRel;
  struct NQDBCSectionAttrDefinition attrDefinition;
  struct NQDBCSectionAttrDefault attrDefault;
};

typedef bool (NQDBCParserCallback) (void* userdata, const NQDBCSection* Section);

typedef struct NQDBCParser NQDBCParser;

NQ_EXPORT NQDBCParser* NQDBCParser_create(void* userdata, NQDBCParserCallback* callback);
NQ_EXPORT NQDBCParser* NQDBCParser_create2(void* userdata, NQDBCParserCallback* callback, void* allocator, NQDBCAllocFn* alloc, NQDBCFreeFn* free);
NQ_EXPORT void NQDBCParser_destroy(NQDBCParser*);

NQ_EXPORT bool NQDBCParser_append(NQDBCParser*, const char* data, size_t size);
NQ_EXPORT bool NQDBCParser_finish(NQDBCParser*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCPARSER_H */
