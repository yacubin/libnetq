/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/dbc/DBCDocBuilder.h"

NQDBCDocBuilder* NQDBCDocBuilder_create(void)
{
  // Add psevdo message `Vector__XXX`
  return NULL;
}

void NQDBCDocBuilder_destroy(NQDBCDocBuilder* thiz)
{
  /* Not implemented */
}

void NQDBCDocBuilder_setVersion(NQDBCDocBuilder* thiz, const char* version)
{
  /* Not implemented */
}

void NQDBCDocBuilder_setSymbols(NQDBCDocBuilder* thiz, const char** symbols, size_t count)
{
  /* Not implemented */
}

void NQDBCDocBuilder_setBitTiming(NQDBCDocBuilder* thiz, uint32_t baudrate, uint32_t btr1, uint32_t btr2)
{
  /* Not implemented */
}

void NQDBCDocBuilder_setNetNodes(NQDBCDocBuilder* thiz, const char** nodes, size_t count)
{
  /* Not implemented */
}

void NQDBCDocBuilder_addValTable(NQDBCDocBuilder* thiz, const char* name, const NQDBCValDesc* desc, size_t count)
{
  /* Not implemented */
}

void NQDBCDocBuilder_addMessage(NQDBCDocBuilder* thiz, uint32_t id, const char* name, uint32_t messageSize, const char* transmitter)
{
  /* Not implemented */
}

void NQDBCDocBuilder_addMessageTransmitter(NQDBCDocBuilder* thiz, uint32_t messageId, const char** transmitters, size_t count)
{
  /* Not implemented */
}
