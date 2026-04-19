/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCERRORCODE_H
#define _LIBNETQ_DBC_DBCERRORCODE_H

typedef enum NQDBCErrorCode {
  kNQDBCErrorInternal = 1,
  kNQDBCErrorUser,
  kNQDBCErrorBufferOverflow,
  kNQDBCErrorStackOverflow,

  kNQDBCErrorSectionKeyword,
  kNQDBCErrorSectionVersion,
  kNQDBCErrorSectionNewSymbols,
  kNQDBCErrorSectionBitTiming,
  kNQDBCErrorSectionNetNodes,
  kNQDBCErrorSectionValTable,
  kNQDBCErrorSectionMessage,
  kNQDBCErrorSectionSignal,
  kNQDBCErrorSectionSignalType,
  kNQDBCErrorSectionSignalValue,
  kNQDBCErrorSectionSignalValueType,
  kNQDBCErrorSectionSignalTypeRef,
  kNQDBCErrorSectionSignalGroup,
  kNQDBCErrorSectionMessageTransmitter,
  kNQDBCErrorSectionEnvVar,
  kNQDBCErrorSectionEnvVarData,
  kNQDBCErrorSectionEnvVarValue,
  kNQDBCErrorSectionComment,
  kNQDBCErrorSectionValueDescription,
  kNQDBCErrorSectionAttrDefinition,
  kNQDBCErrorSectionAttrRelDefinition,
  kNQDBCErrorSectionAttrDefault,
  kNQDBCErrorSectionAttrRelDefault,
  kNQDBCErrorSectionAttrValue,
  kNQDBCErrorSectionAttrRelValue,
  kNQDBCErrorSectionMultiplexedSignal,

  kNQDBCErrorTokenNumber,
  kNQDBCErrorTokenString,
} NQDBCErrorCode;

#endif /* _LIBNETQ_DBC_DBCERRORCODE_H */
