/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCDOCBUILDER_H
#define _LIBNETQ_DBC_DBCDOCBUILDER_H

#include <libnetq/dbc/DBCTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct NQDBCDocBuilder NQDBCDocBuilder;

NQ_EXPORT NQDBCDocBuilder* NQDBCDocBuilder_create(void);
NQ_EXPORT void NQDBCDocBuilder_destroy(NQDBCDocBuilder*);
NQ_EXPORT void NQDBCDocBuilder_setVersion(NQDBCDocBuilder*, const char* version);
NQ_EXPORT void NQDBCDocBuilder_setSymbols(NQDBCDocBuilder*, const char** symbols, size_t count);
NQ_EXPORT void NQDBCDocBuilder_setBitTiming(NQDBCDocBuilder*, uint32_t baudrate, uint32_t btr1, uint32_t btr2);
NQ_EXPORT void NQDBCDocBuilder_setNetNodes(NQDBCDocBuilder*, const char** nodes, size_t count);
NQ_EXPORT void NQDBCDocBuilder_addValTable(NQDBCDocBuilder*, const char* name, const NQDBCValDesc* desc, size_t count);
NQ_EXPORT void NQDBCDocBuilder_addMessage(NQDBCDocBuilder*, uint32_t id, const char* name, uint32_t messageSize, const char* transmitter);
NQ_EXPORT void NQDBCDocBuilder_addMessageTransmitter(NQDBCDocBuilder*, uint32_t messageId, const char** transmitters, size_t count);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCDOCBUILDER_H */
