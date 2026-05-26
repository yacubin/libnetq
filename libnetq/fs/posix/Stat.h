/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_FS_POSIX_STAT_H
#define _LIBNETQ_FS_POSIX_STAT_H

#include <libnetq/Time.h>

#ifdef NQ_OS_UNIX

# include <sys/stat.h>
# include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_STAT_IFMT  S_IFMT
#define NQ_STAT_IFREG S_IFREG
#define NQ_STAT_IFDIR S_IFDIR
#define NQ_STAT_IFLNK S_IFLNK

typedef struct stat NQStat;

static inline int NQGetStat(const char* path, NQStat* st)
{
  if (stat(path, st) != 0)
    return -errno;
  return 0;
}

static inline bool NQStat_isFile(const NQStat* st)
{
  return S_ISREG(st->st_mode);
}
static inline bool NQStat_isDirectory(const NQStat* st)
{
  return S_ISDIR(st->st_mode);
}
static inline bool NQStat_isSymbolicLink(const NQStat* st)
{
  return S_ISLNK(st->st_mode);
}
static inline uint64_t NQStat_size(const NQStat* st)
{
  return (uint64_t)st->st_size;
}
static inline NQTimeMs NQStat_accesseTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->st_atim);
}
static inline NQTimeMs NQStat_modificationTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->st_mtim);
}
static inline NQTimeMs NQStat_creationTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->st_ctim);
}

#ifdef __cplusplus
}
#endif
#endif

#endif /* _LIBNETQ_FS_POSIX_STAT_H */
