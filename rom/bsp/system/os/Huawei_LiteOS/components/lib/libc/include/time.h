/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 * Copyright (c) <2014-2015>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

 /*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
/** @defgroup time Time
  * @ingroup posix
  */
#ifndef __TIME_H__
#define __TIME_H__

#include "los_typedef.h"
#include "float.h"
#include "sys/types.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


extern  int _daylight;


/**
 *  @ingroup time
 *  The up-limit of number of timer.
 */
#define _POSIX_TIMER_MAX	32

/**
 *  @ingroup time
 *  Maximum length of a timezone name (element of `tzname').
 */
#define TZ_PATH_LENTGH 100

/*---------------------------------------------------------------------------*/
/* Types for timers and clocks */

/**
 *  @ingroup time
 *  Define clockid_t as an integer.
 */
typedef int clockid_t;

/**
 *  @ingroup time
 *  Define time_t as an integer.
 */
typedef long time_t;

/**
 *  @ingroup time
 *  Define clock_t as a long integer.
 */
typedef long clock_t;

/**
 *  @ingroup time
 *  Define timer_t as a pointer to software timer control structure.
 */
typedef struct tagSwTmrCtrl *timer_t;

/**
 *  @ingroup time
 *  Signal struct.
 */
//struct sigevent;

/*---------------------------------------------------------------------------*/
/* Structures */

/**
 *  @ingroup time
 *  Time struct.
 */
struct timespec
{
    int         tv_sec;      /**< second */
    long        tv_nsec;     /**< nanosecond */
};
/**
 *  @ingroup time
 *  Time struct of timer.
 */
struct itimerspec
{
    struct timespec     it_interval;/**< timer interval */
    struct timespec     it_value;   /**< initial expiration */
};
/*
 * Structure returned by gettimeofday(2) system call,
 * and used in other calls.
 */
/**
 * @ingroup time
 * Time struct.
 */
struct timeval {
    long tv_sec;    /**< second */
    long tv_usec;   /**< microseconds */
};

/**
 *  @ingroup time
 *  Time zone number : no-use.
 */
#define DST_NONE    0
/**
 *  @ingroup time
 *  Time zone number : U.S.A.
 */
#define DST_USA     1
/**
 *  @ingroup time
 *  Time zone number : Australia.
 */
#define DST_AUST    2
/**
 *  @ingroup time
 *  Time zone number : Western Europe.
 */
#define DST_WET     3
/**
 *  @ingroup time
 *  Time zone number : Central Europe.
 */
#define DST_MET     4
/**
 *  @ingroup time
 *  Time zone number : Eastern Europe.
 */
#define DST_EET     5
/**
 *  @ingroup time
 *  Time zone number : Canada.
 */
#define DST_CAN     6

/**
 * @ingroup time
 * time structure
 */
struct tm {
   int     tm_sec;         /**< The number of seconds after the minute, normally in the range 0 to 59, but can be up to 60 to allow for leap seconds */
   int     tm_min;         /**< The number of minutes after the hour, in the range 0 to 59 */
   int     tm_hour;        /**< The number of hours past midnight, in the range 0 to 23 */
   int     tm_mday;        /**< The day of the month, in the range 1 to 31 */
   int     tm_mon;         /**< The number of months since January, in the range 0 to 11 */
   int     tm_year;        /**< The number of years since 1900. */
   int     tm_wday;        /**< The number of days since Sunday, in the range 0 to 6 */
   int     tm_yday;        /**< The number of days since January 1, in the range 0 to 365 */
   int     tm_isdst;       /**< A flag that indicates whether daylight saving time is in effect at the time described. The value is positive if daylight saving time is in effect, zero if it is not, and negative if the information is not available */
};

/**
 * @ingroup timezone
 * timezone structure
 */
struct timezone{
    int tz_minuteswest;/* time difference from Greenwich mean time to the West */
    int tz_dsttime;/* modified way of DST  time */
};

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed in Coordinated Universal Time (UTC).</li>
 *</ul>
 *@attention
 *<ul>
 *<li> It may return NULL when the year does not fit into an integer.</li>
 *<li> The return value points to a statically allocated struct which might be overwritten by subsequent calls to any of the date and time functions, Thread-safe versions is gmtime_r().</li>
 *</ul>
 *
 *@param timep   [IN] Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *
 *@retval tm*       Succeed, points to a statically allocated struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gmtime_r, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  gmtime(const time_t *timep);

/**
 *@ingroup time
 *@brief Converts the calendar time timep to broken-down time representation.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to converts the calendar time timep to broken-down time representation, expressed in Coordinated Universal Time (UTC).</li>
 *</ul>
 *@attention
 *<ul>
 *<li> It may return NULL when the year does not fit into an integer.</li>
 *<li> The user-supplied struct specified by result can not be NULL.</li>
 *</ul>
 *
 *@param timep   [IN]  Represents calendar time, When interpreted as an absolute time value, it represents the number of seconds elapsed since the Epoch, 1970-01-01 00:00:00 +0000 (UTC).
 *@param result  [OUT] Point to the user-supplied struct where to stores the data.
 *
 *@retval tm*       Succeed, points to the user-supplied tm struct.
 *@retval NULL      Failed.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see gmtime, asctime
 *@since Huawei LiteOS V100R001C00
 */
extern struct tm*  gmtime_r(const time_t *timep, struct tm *result);
/**
 *@ingroup time
 *@brief Set the time of the specified clock clock_id.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Set the time of the specified clock clock_id.</li>
 *</ul>
 *
 *@param clock_id   [IN]  The identifier of the particular clock on which to 
act, only support CLOCK_REALTIME.
 *@param tp         [IN]  Point to the timespec structure.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see clock_gettime
 *@since Huawei LiteOS V100R001C00
 */
extern LITE_OS_SEC_TEXT_MINOR int clock_settime(clockid_t clock_id, const struct timespec *tp);
/**
 *@ingroup time
 *@brief Retrieve the time of the specified clock clock_id.
 *
 *@par Description:
 *<ul>
 *<li> This API is used to Retrieve the time of the specified clock clock_id
.</li>
 *</ul>
 *
 *@param clock_id   [IN]  The identifier of the particular clock on which to 
act.
 *@param tp         [OUT] Point to the timespec structure.
 *
 *@retval  0    Succeed.
 *@retval -1    Failed, with any of the following error codes in the errno:
                #EINVAL     : Invalid parameter.
 *@par Dependency:
 *<ul><li>time.h The header file that contains the API declaration.</li></ul>
 *@see clock_settime
 *@since Huawei LiteOS V100R001C00
 */
extern LITE_OS_SEC_TEXT_MINOR int clock_gettime(clockid_t clock_id, struct timespec *tp);
extern int gettimeofday(struct timeval* tv, struct timezone* tz);

/* global includes */
extern int       daylight;

/* Identifier for system-wide realtime clock.  */
#define CLOCK_REALTIME              0
/* Monotonic system-wide clock.  */
#define CLOCK_MONOTONIC             1
/* High-resolution timer from the CPU.  */
#define CLOCK_PROCESS_CPUTIME_ID    2
/* Thread-specific CPU-time clock.  */
#define CLOCK_THREAD_CPUTIME_ID     3
#define CLOCK_MONOTONIC_RAW         4
#define CLOCK_REALTIME_COARSE       5
#define CLOCK_MONOTONIC_COARSE      6
#define CLOCK_BOOTTIME              7
#define CLOCK_REALTIME_ALARM        8
#define CLOCK_BOOTTIME_ALARM        9

#define SECSPERMIN                  60
#define MINSPERHOUR                 60
#define HOURSPERDAY                 24
#define DAYSPERWEEK                 7
#define DAYSPERNYEAR                365
#define DAYSPERLYEAR                366
#define SECSPERHOUR                 (SECSPERMIN * MINSPERHOUR)
#define SECSPERDAY                  ((int) SECSPERHOUR * HOURSPERDAY)
#define MONSPERYEAR                 12

#define YEAR_BASE	1900
#define EPOCH_YEAR      1970
#define EPOCH_WDAY      TM_THURSDAY
#define EPOCH_YEARS_SINCE_LEAP 2
#define EPOCH_YEARS_SINCE_CENTURY 70
#define EPOCH_YEARS_SINCE_LEAP_CENTURY 370




#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
