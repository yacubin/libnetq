/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_CHECKSUM_CRC32_H
#define _LIBNETQ_CHECKSUM_CRC32_H

#include <libnetq/Basic.h>

#ifdef NQ_SYS_LINUX
# include <libnetq/checksum/CRC32SysLinux.h>
#elif USE_ZLIB_CRC32
# include <libnetq/checksum/CRC32ZLib.h>
#else
# include <libnetq/checksum/CRC32Stub.h>
#endif

#endif /* _LIBNETQ_CHECKSUM_CRC32_H */
