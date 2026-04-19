/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#include "config.h"
#include "libnetq/dbc/DBCID.h"

uint32_t NQDBCGetIdBits(NQDBCID id)
{
  return NQDBCIsExtendedId(id) ? NQDBC_EXTENDED_ID_BITS : NQDBC_BASE_ID_BITS;
}
