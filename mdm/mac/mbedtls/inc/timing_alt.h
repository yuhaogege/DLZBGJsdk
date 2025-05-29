/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: Timing function.
 */

#ifndef __TIMING_ALT_H__
#define __TIMING_ALT_H__

#include "stdint.h"

/**
 * @ingroup timezone
 * timezone structure
 */
struct timezone {
    int tz_minuteswest;     /**< The difference between Greenwich Mean Time and the time of the west. */
    int tz_dsttime;         /**< DST time correction mode */
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

struct _hr_time {
    struct timeval start;
};

struct mbedtls_timing_hr_time {
    unsigned char opaque[32];
};

/**
 * \brief          Context for mbedtls_timing_set/get_delay()
 */
typedef struct mbedtls_timing_delay_context {
    struct mbedtls_timing_hr_time   timer;
    uint32_t                        int_ms;
    uint32_t                        fin_ms;
} mbedtls_timing_delay_context;

int gettimeofday(struct timeval* tv, struct timezone* tz);

#endif
