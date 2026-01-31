/*
 * MIT License
 *
 * Copyright (c) 2026  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_FS_STAT_H
#define _LIBNETQ_FS_STAT_H

#include <libnetq/Time.h>

#ifdef NQ_SYS_LINUX
# include <linux/stat.h>
#endif

#ifdef NQ_OS_UNIX
# include <sys/stat.h>
# include <errno.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NQ_SYS_LINUX

#define NQ_STAT_IFMT  S_IFMT
#define NQ_STAT_IFREG S_IFREG
#define NQ_STAT_IFDIR S_IFDIR
#define NQ_STAT_IFLNK S_IFLNK

typedef struct kstat NQStat;

NQ_EXPORT int NQGetStat(const char* path, NQStat* st);

static inline bool NQStat_isFile(const NQStat* st)
{
  return S_ISREG(st->mode);
}
static inline bool NQStat_isDirectory(const NQStat* st)
{
  return S_ISDIR(st->mode);
}
static inline bool NQStat_isSymbolicLink(const NQStat* st)
{
  return S_ISLNK(st->mode);
}
static inline uint64_t NQStat_size(const NQStat* st)
{
  return (uint64_t)st->size;
}
static inline NQTimeMs NQStat_accesseTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->atime);
}
static inline NQTimeMs NQStat_modificationTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->mtime);
}
static inline NQTimeMs NQStat_creationTimeMs(const NQStat* st)
{
  return NQTimeSpecToTimeMs(&st->ctime);
}

#endif

#ifdef NQ_OS_UNIX

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

#endif

#ifdef NQ_OS_WINDOWS

#define NQ_STAT_IFMT  0170000
#define NQ_STAT_IFREG 0100000
#define NQ_STAT_IFDIR 0040000
#define NQ_STAT_IFLNK 0120000

typedef struct NQStat NQStat;
struct NQStat {
  uint32_t mode;
  uint64_t size;
  NQTimeMs accesseTimeMs;
  NQTimeMs modificationTimeMs;
  NQTimeMs creationTimeMs;
};

NQ_EXPORT int NQGetStat(const char* path, NQStat* st);

static inline bool NQStat_isFile(const NQStat* st)
{
  return (st->mode & NQ_STAT_IFMT) == NQ_STAT_IFREG;
}
static inline bool NQStat_isDirectory(const NQStat* st)
{
  return (st->mode & NQ_STAT_IFMT) == NQ_STAT_IFDIR;
}
static inline bool NQStat_isSymbolicLink(const NQStat* st)
{
  return (st->mode & NQ_STAT_IFMT) == NQ_STAT_IFLNK;
}
static inline size_t NQStat_size(const NQStat* st)
{
  return st->size;
}
static inline NQTimeMs NQStat_accesseTimeMs(const NQStat* st)
{
  return st->accesseTimeMs;
}
static inline NQTimeMs NQStat_modificationTimeMs(const NQStat* st)
{
  return st->modificationTimeMs;
}
static inline NQTimeMs NQStat_creationTimeMs(const NQStat* st)
{
  return st->creationTimeMs;
}

#endif

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_FS_STAT_H */
