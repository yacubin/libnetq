/*
 * MIT License
 *
 * Copyright (c) 2025-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_DBC_DBCDOCTOJSONWRITER_H
#define _LIBNETQ_DBC_DBCDOCTOJSONWRITER_H

#include <libnetq/json/JSONWriter.h>
#include <libnetq/dbc/DBCDocument.h>

#ifdef __cplusplus
extern "C" {
#endif

NQ_EXPORT bool NQDBCAttributeWriteTo(const NQDBCAttribute*, NQJSONWriter* writer);
NQ_EXPORT bool NQDBCDocumentWriteTo(const NQDBCDocument*, const char* filename, NQJSONWriter* writer);
NQ_EXPORT bool NQDBCMessageWriteTo(const NQDBCMessage*, bool isJ1939, NQJSONWriter* writer);
NQ_EXPORT bool NQDBCSignalWriteTo(const NQDBCSignal*, NQJSONWriter* writer);
NQ_EXPORT bool NQDBCSigGroupWriteTo(const NQDBCSigGroup*, NQJSONWriter* writer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_DBC_DBCDOCTOJSONWRITER_H */
