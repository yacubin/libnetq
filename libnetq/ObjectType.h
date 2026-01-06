/*
 * MIT License
 *
 * Copyright (c) 2020-2026  Yurii Yakubin (yurii.yakubin@gmail.com)
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
  NQLogCircularObjectType = 1004,
  NQHexViewerObjectType = 1005,
  NQUint8ArrayObjectType = 1006,
  NQAssetSystemObjectType = 1009,
  NQSocketObjectType = 1010,
  NQLooperSourceObjectType = 1014,
  NQArgumentsObjectType = 1016,
  kNQKeyValObjectType = 1017,
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _LIBNETQ_OBJECTTYPE_H */
