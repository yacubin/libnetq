/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_OBJECTTYPE_H
#define _LIBNETQ_OBJECTTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

enum NQObjectType {
  NQThreadObjectType = 1000,
  NQDirObjectType = 1001,
  NQDescSetObjectType = 1002,
  NQWebSocketObjectType = 1003,
  NQLogCircularObjectType = 1004,
  NQHexViewerObjectType = 1005,
  NQUint8ArrayObjectType = 1006,
  NQAssetDirObjectType = 1007,
  NQAssetFileObjectType = 1008,
  NQAssetSystemObjectType = 1009,
  NQSocketObjectType = 1010,
  NQTLSSocketObjectType = 1011,
  NQTimerQueueObjectType = 1012,
  NQDispatchQueueObjectType = 1013,
  NQLooperSourceObjectType = 1014,
  NQURLObjectType = 1015,
  NQArgumentsObjectType = 1016,
  kNQKeyValObjectType = 1017,
  kNQAssetBinaryObjectType = 1018,
  kNQAssetDirBinaryObjectType = 1019,
  kNQAssetFileBinaryObjectType = 1020,
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_OBJECTTYPE_H */
