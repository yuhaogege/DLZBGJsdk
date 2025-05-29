/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: soc_mid.
 * Author: CompanyName
 * Create: 2018-10-31
 */
/**
 * @defgroup hct_mid Middle layer init.
 * @ingroup mid
 */

#ifndef __SOC_MID_H__
#define __SOC_MID_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_mid
 * @brief  Middle layer initializes.
 *
 * @par Description:
 * Middle layer initializes.
 *
 * @attention
 * @li This API needs to be called in the app startup process.
 * @li This API is a middle layer resource initialization function and cannot be called multiple times.
 * @par Dependency:
 * @li soc_mid.h: This file describes the APIs for the middle layer initialization.
 * @see None
 * @since DW21_V100R001C00
 */
td_void uapi_mid_init(td_void);

#ifdef __cplusplus
}
#endif
#endif /* __SOC_MID_H__ */
