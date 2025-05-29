/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: tsensor driver implementatioin's inner head file.
 * Author: CompanyName
 * Create: 2019-06-03
 */

#ifndef __TSENSOR_H__
#define __TSENSOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TSENSOR_IP_CTL                  EXT_TSENSOR_REG_BASE
#define TSENSOR_LIMIT_CTL               0x04
#define TSENSOR_TEMPEN_CTL              0x08
#define TSENSOR_TEMP0                   0x0C
#define TSENSOR_TEMP1                   0x10
#define TSENSOR_TEMP2                   0x14
#define TSENSOR_TEMP3                   0x18
#define TSENSOR_TEMPEN_CTL_LOOP         0
#define TSENSOR_TEMPEN_CTL_SINGLE       1
#define TSENSOR_TEMPEN_CTL_CNT16_BYPASS 3

#define TSENSOR_MODE_16_POINTS_SINGLE 0       /* Mode: 16 points average single report */
#define TSENSOR_MODE_16_POINTS_LOOP   1       /* Mode: 16-point average loop report */
#define TSENSOR_MODE_1_POINT_LOOP     2       /* Mode: Single point loop report */
#define TSENSOR_TEMPERATURE_HIGH      125.0   /* Maximum temperature */
#define TSENSOR_TEMPERATURE_LOW       (-40.0) /* Temperature minimum */
#define TSENSOR_TEMPERATURE_D_VALUE   (TSENSOR_TEMPERATURE_HIGH - TSENSOR_TEMPERATURE_LOW)
#define TSENSOR_HIGH_CODE_VALUE       933 /* The code value corresponding to the temperature maximum */
#define TSENSOR_LOW_CODE_VALUE        118 /* The code value corresponding to the temperature minimum */

#define TSENSOR_CODE_D_VALUE (TSENSOR_HIGH_CODE_VALUE - TSENSOR_LOW_CODE_VALUE)
#define TSENSOR_DLY_20MS     3

#ifdef TSENSOR_DEBUG_PRINT
#define tsensor_debug(msg...)    \
    do {                         \
        dprintf("\n"); \
        dprintf("<tsensor>:"); \
        dprintf(msg);            \
        dprintf("\n"); \
    } while (0)
#else
#define tsensor_debug(msg...)
#endif

#ifdef __cplusplus
}
#endif
#endif
