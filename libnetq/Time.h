/*
 * MIT License
 *
 * Copyright (c) 2020-2025  Yurii Yakubin (yurii.yakubin@gmail.com)
 *
 * Permission is granted to use, copy, modify, and distribute this software
 * under the MIT License. See LICENSE file for details.
 */

#ifndef _LIBNETQ_TIME_H
#define _LIBNETQ_TIME_H

#include <time.h>

#include <libnetq/Basic.h>
#include <libnetq/Limits.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NQ_HOURS_PER_DAY  24
#define NQ_SECS_PER_MIN   60
#define NQ_MINS_PER_HOUR  60
#define NQ_MSECS_PER_SEC  1000
#define NQ_DAYS_PER_WEEK  7
#define NQ_DAYS_PER_NYEAR 365
#define NQ_DAYS_PER_LYEAR 366
#define NQ_SECS_PER_HOUR (NQ_SECS_PER_MIN * NQ_MINS_PER_HOUR)
#define NQ_MONS_PER_YEAR  12
#define NQ_TM_YEAR_BASE   1900
#define NQ_EPOCH_YEAR     1970

#define NQIsLeapYear(y) (((y) % 4) == 0 && (((y) % 100) != 0 || ((y) % 400) == 0))

// NQDataTime.h
typedef struct NQDataTime {
  int year;
  int month;
  int yearDay;
  int monthDay;
  int weekDay;
  int hour;
  int minute;
  int second;
  int utcOffset;
  int isDST;
} NQDataTime;

void NQDataTime_init(NQDataTime*);
void NQDataTime_initLocalTime(NQDataTime*);

#define NQ_TIME_MAX NQ_INT64_MAX

typedef int64_t NQTime; // in milliseconds
typedef int64_t NQMonoTime;
typedef int64_t NQMSec;
typedef uint64_t NQTick; // in milliseconds
typedef int64_t NQUnixTime; // in seconds
// NQMonoTime

typedef double NQSeconds;
NQ_EXPORT NQTime NQGetTime();
NQ_EXPORT NQTick NQGetCPUTick();

static NQ_ALWAYS_INLINE NQUnixTime NQGetUnixTime()
{
  return NQGetTime() / NQ_MSECS_PER_SEC;
}

NQ_EXPORT void NQGetLocaltime(const time_t* t, struct tm* tm);
NQ_EXPORT void nq_gmtimems(NQTime time, struct tm* ptm, int* pms);
NQ_EXPORT void nq_gmtime(const time_t* t, struct tm* tm);
NQ_EXPORT time_t nq_timegm(const struct tm* tm);

enum NQTimeFormat {
  NQ_DT_UNIXEPOCH,           // 1539993600
  NQ_DT_UNIXEPOCHMS,         // 1539993600020
  NQ_DT_RFC2445,             // 19980119 070000
  NQ_DT_RFC2445TZ,           // 19980119T070000Z
  NQ_DT_RFC3339,             // 2019-10-12 07:20:50
  NQ_DT_RFC3339TZ,           // 2019-10-12T07:20:50.529Z
  NQ_DT_RFC3339_TIME,        // 07:20:50
  NQ_DT_RFC3339_TIMEMS,      // 07:20:50.433
                              // 2019-10-12T07:20:50Z (ISO8601)
};

/*
  NQ_DT_RFC2445 | NQ_DT_DATE | NQ_DT_TIME | NQ_DT_MS | NQ_DT_TZ
  NQ_DT_RFC3339 | NQ_DT_DATE | NQ_DT_TIME | NQ_DT_MS | NQ_DT_TZ
*/

NQ_EXPORT size_t NQTimeFormat(NQTime time, int format, char* buffer, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* _LIBNETQ_TIME_H */
