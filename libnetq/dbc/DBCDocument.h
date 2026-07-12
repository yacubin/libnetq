/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCDOCUMENT_H
#define _LIBNETQ_DBC_DBCDOCUMENT_H

#include <libnetq/dbc/DBCTypes.h>
#include <libnetq/dbc/DBCAlloc.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQDBCDocument NQDBCDocument;
typedef struct NQDBCAttrProto NQDBCAttrProto;
typedef struct NQDBCAttribute NQDBCAttribute;
typedef struct NQDBCNetNode NQDBCNetNode;
typedef struct NQDBCMessage NQDBCMessage;
typedef struct NQDBCSignal NQDBCSignal;
typedef struct NQDBCSigGroup NQDBCSigGroup;
typedef struct NQDBCSigType NQDBCSigType;
typedef struct NQDBCEnvVar NQDBCEnvVar;
typedef struct NQDBCValTable NQDBCValTable;

/* NQDBCAttrProto */

NQ_EXPORT bool NQDBCAttrProto_isRelateDocument(const NQDBCAttrProto*);
NQ_EXPORT bool NQDBCAttrProto_isRelateNetNode(const NQDBCAttrProto*);
NQ_EXPORT bool NQDBCAttrProto_isRelateMessage(const NQDBCAttrProto*);
NQ_EXPORT bool NQDBCAttrProto_isRelateSignal(const NQDBCAttrProto*);
NQ_EXPORT bool NQDBCAttrProto_isRelateEnvVar(const NQDBCAttrProto*);

NQ_EXPORT bool NQDBCAttrProto_hasDefault(const NQDBCAttrProto*);
NQ_EXPORT bool NQDBCAttrProto_setDefaultInt(NQDBCAttrProto*, int64_t value);
NQ_EXPORT bool NQDBCAttrProto_setDefaultFloat(NQDBCAttrProto*, double value);
NQ_EXPORT bool NQDBCAttrProto_setDefaultString(NQDBCAttrProto*, const char* value);

/* NQDBCAttribute */

NQ_EXPORT NQDBCAttrValueType NQDBCAttribute_type(const NQDBCAttribute*);
NQ_EXPORT const char* NQDBCAttribute_name(const NQDBCAttribute*);

NQ_EXPORT int64_t NQDBCAttribute_asIntValue(const NQDBCAttribute*);
NQ_EXPORT double NQDBCAttribute_asFloatValue(const NQDBCAttribute*);
NQ_EXPORT const char* NQDBCAttribute_asStringValue(const NQDBCAttribute*);

/* NQDBCDocument */

NQ_EXPORT NQDBCDocument* NQDBCDocument_create(const char* name);
NQ_EXPORT NQDBCDocument* NQDBCDocument_create2(const char* name, void* allocator, NQDBCAllocFn* alloc, NQDBCFreeFn* free);
NQ_EXPORT void NQDBCDocument_release(NQDBCDocument*);

NQ_EXPORT NQDBCNetNode* NQDBCDocument_createNetNode(NQDBCDocument*, const char* name);
NQ_EXPORT NQDBCMessage* NQDBCDocument_createMessage(NQDBCDocument*, const char* name, uint32_t id, uint32_t sizeInBytes);
NQ_EXPORT NQDBCSignal* NQDBCDocument_createSignal(NQDBCDocument*, const char* name, uint32_t startBit, const NQDBCSigInfo* info);
NQ_EXPORT NQDBCEnvVar* NQDBCDocument_createEnvVar(NQDBCDocument*, const char* name, const NQDBCEnvInfo* info);
NQ_EXPORT NQDBCSigGroup* NQDBCDocument_createSigGroup(NQDBCDocument*, const char* name, uint32_t repetitions);
NQ_EXPORT NQDBCValTable* NQDBCDocument_createValTable(NQDBCDocument*, const char* name, const NQDBCValDesc* items, size_t size);
NQ_EXPORT NQDBCSigType* NQDBCDocument_createSigType(NQDBCDocument*, const char* name, const NQDBCSigInfo* info, double defaultValue);

NQ_EXPORT NQDBCNetNode* NQDBCDocument_findNetNode(NQDBCDocument*, const char* name);
NQ_EXPORT NQDBCMessage* NQDBCDocument_findMessage(NQDBCDocument*, uint32_t id);
NQ_EXPORT NQDBCSignal* NQDBCDocument_findSignal(NQDBCDocument*, uint32_t messageId, const char* signalName);
NQ_EXPORT NQDBCEnvVar* NQDBCDocument_findEnvVar(NQDBCDocument*, const char* name);
NQ_EXPORT NQDBCValTable* NQDBCDocument_findValTable(NQDBCDocument*, const char* name);
NQ_EXPORT NQDBCSigType* NQDBCDocument_findSigType(NQDBCDocument*, const char* name);

NQ_EXPORT const char* NQDBCDocument_name(const NQDBCDocument*);

NQ_EXPORT const char* NQDBCDocument_comment(const NQDBCDocument*);
NQ_EXPORT bool NQDBCDocument_setComment(NQDBCDocument*, const char* comment);

NQ_EXPORT const NQDBCBitTiming* NQDBCDocument_bitTiming(const NQDBCDocument*);
NQ_EXPORT size_t NQDBCDocument_messageCount(const NQDBCDocument*);
NQ_EXPORT NQDBCMessage* NQDBCDocument_messageAt(const NQDBCDocument*, size_t index);

NQ_EXPORT NQDBCAttribute* NQDBCDocument_findAttribute(const NQDBCDocument*, const char* name);
NQ_EXPORT size_t NQDBCDocument_attributeCount(const NQDBCDocument*);
NQ_EXPORT NQDBCAttribute* NQDBCDocument_attributeAt(const NQDBCDocument*, size_t index);

NQ_EXPORT bool NQDBCDocument_getAttrInt(const NQDBCDocument*, const char* name, int64_t* result);
NQ_EXPORT bool NQDBCDocument_setAttrInt(NQDBCDocument*, const char* name, int64_t value);
NQ_EXPORT bool NQDBCDocument_getAttrFloat(const NQDBCDocument*, const char* name, double* result);
NQ_EXPORT bool NQDBCDocument_setAttrFloat(NQDBCDocument*, const char* name, double value);
NQ_EXPORT bool NQDBCDocument_getAttrString(const NQDBCDocument*, const char* name, const char** result);
NQ_EXPORT bool NQDBCDocument_setAttrString(NQDBCDocument*, const char* name, const char* value);

NQ_EXPORT const char* NQDBCDocument_version(const NQDBCDocument* thiz);
NQ_EXPORT bool NQDBCDocument_setVersion(NQDBCDocument* thiz, const char* version);

NQ_EXPORT size_t NQDBCDocument_symbolCount(const NQDBCDocument*);
NQ_EXPORT const char* NQDBCDocument_symbolAt(const NQDBCDocument*, size_t index);
NQ_EXPORT bool NQDBCDocument_setSymbols(NQDBCDocument*, const char** symbols, size_t count);

NQ_EXPORT void NQDBCDocument_setBitTiming(NQDBCDocument*, const NQDBCBitTiming* bitTiming);

NQ_EXPORT bool NQDBCDocument_addNetNode(NQDBCDocument*, NQDBCNetNode* netNode);
NQ_EXPORT bool NQDBCDocument_addMessage(NQDBCDocument*, NQDBCMessage* message);
NQ_EXPORT bool NQDBCDocument_addEnvVar(NQDBCDocument*, NQDBCEnvVar* envVar);
NQ_EXPORT bool NQDBCDocument_addValTable(NQDBCDocument*, NQDBCValTable* valTable);
NQ_EXPORT bool NQDBCDocument_addSigType(NQDBCDocument*, NQDBCSigType* sigType);

NQ_EXPORT bool NQDBCDocument_defineDocumentAttr(NQDBCDocument*, const char* name, const struct NQDBCAttrValueInfo* params);
NQ_EXPORT bool NQDBCDocument_defineNetNodeAttr(NQDBCDocument*, const char* name, const struct NQDBCAttrValueInfo* params);
NQ_EXPORT bool NQDBCDocument_defineMessageAttr(NQDBCDocument*, const char* name, const struct NQDBCAttrValueInfo* params);
NQ_EXPORT bool NQDBCDocument_defineSignalAttr(NQDBCDocument*, const char* name, const struct NQDBCAttrValueInfo* params);
NQ_EXPORT bool NQDBCDocument_defineEnvVarAttr(NQDBCDocument*, const char* name, const struct NQDBCAttrValueInfo* params);

NQ_EXPORT NQDBCAttrProto* NQDBCDocument_findAttrProto(const NQDBCDocument*, const char* name);
NQ_EXPORT size_t NQDBCDocument_attrProtoCount(const NQDBCDocument*);
NQ_EXPORT NQDBCAttrProto* NQDBCDocument_attrProtoAt(const NQDBCDocument*, size_t index);

/* NQDBCNetNode */

NQ_EXPORT void NQDBCNetNode_release(NQDBCNetNode*);
NQ_EXPORT const char* NQDBCNetNode_name(const NQDBCNetNode*);
NQ_EXPORT NQDBCAttribute* NQDBCNetNode_findAttribute(const NQDBCNetNode*, const char* name);
NQ_EXPORT size_t NQDBCNetNode_attributeCount(const NQDBCNetNode*);
NQ_EXPORT NQDBCAttribute* NQDBCNetNode_attributeAt(const NQDBCNetNode*, size_t index);
NQ_EXPORT bool NQDBCNetNode_setComment(NQDBCNetNode*, const char* comment);

NQ_EXPORT bool NQDBCNetNode_getAttrInt(const NQDBCNetNode*, const char* name, int64_t* result);
NQ_EXPORT bool NQDBCNetNode_setAttrInt(NQDBCNetNode*, const char* name, int64_t value);
NQ_EXPORT bool NQDBCNetNode_getAttrFloat(const NQDBCNetNode*, const char* name, double* result);
NQ_EXPORT bool NQDBCNetNode_setAttrFloat(NQDBCNetNode*, const char* name, double value);
NQ_EXPORT bool NQDBCNetNode_getAttrString(const NQDBCNetNode*, const char* name, const char** result);
NQ_EXPORT bool NQDBCNetNode_setAttrString(NQDBCNetNode*, const char* name, const char* value);

/* NQDBCMessage */

NQ_EXPORT void NQDBCMessage_release(NQDBCMessage*);

NQ_EXPORT const char* NQDBCMessage_name(const NQDBCMessage*);
NQ_EXPORT NQDBCAttribute* NQDBCMessage_findAttribute(const NQDBCMessage*, const char* name);
NQ_EXPORT size_t NQDBCMessage_attributeCount(const NQDBCMessage*);
NQ_EXPORT NQDBCAttribute* NQDBCMessage_attributeAt(const NQDBCMessage*, size_t index);
NQ_EXPORT uint32_t NQDBCMessage_id(const NQDBCMessage*);
NQ_EXPORT uint32_t NQDBCMessage_sizeInBytes(const NQDBCMessage*);
NQ_EXPORT const char* NQDBCMessage_comment(const NQDBCMessage*);
NQ_EXPORT bool NQDBCMessage_isPseudo(const NQDBCMessage*);
NQ_EXPORT NQDBCSignal* NQDBCMessage_findSignal(NQDBCMessage*, const char* name);
NQ_EXPORT size_t NQDBCMessage_groupCount(const NQDBCMessage*);
NQ_EXPORT NQDBCSigGroup* NQDBCMessage_groupAt(const NQDBCMessage*, size_t index);
NQ_EXPORT size_t NQDBCMessage_signalCount(const NQDBCMessage*);
NQ_EXPORT NQDBCSignal* NQDBCMessage_signalAt(const NQDBCMessage*, size_t index);
NQ_EXPORT size_t NQDBCMessage_senderCount(const NQDBCMessage*);
NQ_EXPORT NQDBCNetNode* NQDBCMessage_senderAt(const NQDBCMessage*, size_t index);
NQ_EXPORT bool NQDBCMessage_setComment(NQDBCMessage*, const char* comment);

NQ_EXPORT bool NQDBCMessage_getAttrInt(const NQDBCMessage*, const char* name, int64_t* result);
NQ_EXPORT bool NQDBCMessage_setAttrInt(NQDBCMessage*, const char* name, int64_t value);
NQ_EXPORT bool NQDBCMessage_getAttrFloat(const NQDBCMessage*, const char* name, double* result);
NQ_EXPORT bool NQDBCMessage_setAttrFloat(NQDBCMessage*, const char* name, double value);
NQ_EXPORT bool NQDBCMessage_getAttrString(const NQDBCMessage*, const char* name, const char** result);
NQ_EXPORT bool NQDBCMessage_setAttrString(NQDBCMessage*, const char* name, const char* value);

NQ_EXPORT bool NQDBCMessage_addTransmitter(NQDBCMessage*, NQDBCNetNode* transmitter);
NQ_EXPORT bool NQDBCMessage_addSignal(NQDBCMessage*, NQDBCSignal* signal);
NQ_EXPORT bool NQDBCMessage_addSigGroup(NQDBCMessage*, NQDBCSigGroup* sigGroup);
NQ_EXPORT bool NQDBCMessage_setMultiplexerSignal(NQDBCMessage*, NQDBCSignal* multiplexorSwitch);

/* NQDBCSignal */

NQ_EXPORT void NQDBCSignal_release(NQDBCSignal*);
NQ_EXPORT const char* NQDBCSignal_name(const NQDBCSignal*);
NQ_EXPORT const char* NQDBCSignal_comment(const NQDBCSignal*);
NQ_EXPORT NQDBCAttribute* NQDBCSignal_findAttribute(const NQDBCSignal*, const char* name);
NQ_EXPORT size_t NQDBCSignal_attributeCount(const NQDBCSignal*);
NQ_EXPORT NQDBCAttribute* NQDBCSignal_attributeAt(const NQDBCSignal*, size_t index);
NQ_EXPORT NQDBCSigGroup* NQDBCSignal_sigGroup(const NQDBCSignal*);
NQ_EXPORT NQDBCSigValueType NQDBCSignal_valueType(const NQDBCSignal*);
NQ_EXPORT uint32_t NQDBCSignal_startBit(const NQDBCSignal*);
NQ_EXPORT uint32_t NQDBCSignal_sizeInBits(const NQDBCSignal*);
NQ_EXPORT NQDBCByteOrder NQDBCSignal_byteOrder(const NQDBCSignal*);
NQ_EXPORT bool NQDBCSignal_isUnsigned(const NQDBCSignal*);
NQ_EXPORT double NQDBCSignal_factor(const NQDBCSignal*);
NQ_EXPORT double NQDBCSignal_offset(const NQDBCSignal*);
NQ_EXPORT double NQDBCSignal_minimum(const NQDBCSignal*);
NQ_EXPORT double NQDBCSignal_maximum(const NQDBCSignal*);
NQ_EXPORT const char* NQDBCSignal_unit(const NQDBCSignal*);
NQ_EXPORT size_t NQDBCSignal_receiverCount(const NQDBCSignal*);
NQ_EXPORT NQDBCNetNode* NQDBCSignal_receiverAt(const NQDBCSignal*, size_t index);

NQ_EXPORT bool NQDBCSignal_setComment(NQDBCSignal*, const char* comment);

NQ_EXPORT bool NQDBCSignal_getAttrInt(const NQDBCSignal*, const char* name, int64_t* result);
NQ_EXPORT bool NQDBCSignal_setAttrInt(NQDBCSignal*, const char* name, int64_t value);
NQ_EXPORT bool NQDBCSignal_getAttrFloat(const NQDBCSignal*, const char* name, double* result);
NQ_EXPORT bool NQDBCSignal_setAttrFloat(NQDBCSignal*, const char* name, double value);
NQ_EXPORT bool NQDBCSignal_getAttrString(const NQDBCSignal*, const char* name, const char** result);
NQ_EXPORT bool NQDBCSignal_setAttrString(NQDBCSignal*, const char* name, const char* value);

NQ_EXPORT bool NQDBCSignal_addReceiver(NQDBCSignal*, NQDBCNetNode* receiver);
NQ_EXPORT void NQDBCSignal_setValTable(NQDBCSignal*, NQDBCValTable* valTable);
NQ_EXPORT void NQDBCSignal_setValueType(NQDBCSignal*, NQDBCSigValueType type);
NQ_EXPORT void NQDBCSignal_setSignalType(NQDBCSignal*, NQDBCSigType* sigType);
NQ_EXPORT bool NQDBCSignal_setMultiplexerSignal(NQDBCSignal*, NQDBCSignal* multiplexerSignal);
NQ_EXPORT bool NQDBCSignal_addMultiplexerRange(NQDBCSignal*, uint32_t first, uint32_t second);

/* NQDBCEnvVar */

NQ_EXPORT void NQDBCEnvVar_release(NQDBCEnvVar*);
NQ_EXPORT bool NQDBCEnvVar_setComment(NQDBCEnvVar*, const char* comment);
NQ_EXPORT NQDBCAttribute* NQDBCEnvVar_findAttribute(const NQDBCEnvVar*, const char* name);
NQ_EXPORT size_t NQDBCEnvVar_attributeCount(const NQDBCEnvVar*);
NQ_EXPORT NQDBCAttribute* NQDBCEnvVar_attributeAt(const NQDBCEnvVar*, size_t index);

NQ_EXPORT bool NQDBCEnvVar_getAttrInt(const NQDBCEnvVar*, const char* name, int64_t* result);
NQ_EXPORT bool NQDBCEnvVar_setAttrInt(NQDBCEnvVar*, const char* name, int64_t value);
NQ_EXPORT bool NQDBCEnvVar_getAttrFloat(const NQDBCEnvVar*, const char* name, double* result);
NQ_EXPORT bool NQDBCEnvVar_setAttrFloat(NQDBCEnvVar*, const char* name, double value);
NQ_EXPORT bool NQDBCEnvVar_getAttrString(const NQDBCEnvVar*, const char* name, const char** result);
NQ_EXPORT bool NQDBCEnvVar_setAttrString(NQDBCEnvVar*, const char* name, const char* value);

NQ_EXPORT bool NQDBCEnvVar_addAccessNode(NQDBCEnvVar*, NQDBCNetNode* node);
NQ_EXPORT bool NQDBCEnvVar_setDataSize(NQDBCEnvVar*, uint32_t size);
NQ_EXPORT void NQDBCEnvVar_setValTable(NQDBCEnvVar*, NQDBCValTable* valTable);

/* NQDBCSigGroup */

NQ_EXPORT void NQDBCSigGroup_release(NQDBCSigGroup*);
NQ_EXPORT const char* NQDBCSigGroup_name(const NQDBCSigGroup*);
NQ_EXPORT size_t NQDBCSigGroup_signalCount(const NQDBCSigGroup*);
NQ_EXPORT NQDBCSignal* NQDBCSigGroup_signalAt(const NQDBCSigGroup*, size_t index);
NQ_EXPORT bool NQDBCSigGroup_addSignal(NQDBCSigGroup*, NQDBCSignal* signal);

/* NQDBCValTable */

NQ_EXPORT void NQDBCValTable_release(NQDBCValTable*);

/* NQDBCSigType */

NQ_EXPORT void NQDBCSigType_release(NQDBCSigType*);
NQ_EXPORT void NQDBCSigType_setValTable(NQDBCSigType*, NQDBCValTable* valTable);
NQ_EXPORT NQDBCByteOrder NQDBCSigType_byteOrder(const NQDBCSigType*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCDOCUMENT_H */
