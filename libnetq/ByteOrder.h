/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_BYTEORDER_H
#define _LIBNETQ_BYTEORDER_H

#define NQByteSwap16(x) ((((x) & 0x00FF) << 8) | \
                         (((x) & 0xFF00) >> 8))

#define NQByteSwap32(x) ((((x) & 0x000000FF) << 24) | \
                         (((x) & 0x0000FF00) << 8)  | \
                         (((x) & 0x00FF0000) >> 8)  | \
                         (((x) & 0xFF000000) >> 24))

#define NQByteSwap64(x) ((((x) & 0x00000000000000FFULL) << 56) | \
                         (((x) & 0x000000000000FF00ULL) << 40) | \
                         (((x) & 0x0000000000FF0000ULL) << 24) | \
                         (((x) & 0x00000000FF000000ULL) << 8)  | \
                         (((x) & 0x000000FF00000000ULL) >> 8)  | \
                         (((x) & 0x0000FF0000000000ULL) >> 24) | \
                         (((x) & 0x00FF000000000000ULL) >> 40) | \
                         (((x) & 0xFF00000000000000ULL) >> 56))

#define NQWordSwap32(x) ((((x) & 0x0000FFFF) << 16) | \
                         (((x) & 0xFFFF0000) >> 16))

#define NQWordSwap64(x) ((((x) & 0x000000000000FFFFULL) << 48) | \
                         (((x) & 0x00000000FFFF0000ULL) << 16) | \
                         (((x) & 0x0000FFFF00000000ULL) >> 16) | \
                         (((x) & 0xFFFF000000000000ULL) >> 48))

#define NQDwordSwap64(x) ((((x) & 0x00000000FFFFFFFFULL) << 32) | \
                          (((x) & 0xFFFFFFFF00000000ULL) >> 32))

#endif /* _LIBNETQ_BYTEORDER_H */
