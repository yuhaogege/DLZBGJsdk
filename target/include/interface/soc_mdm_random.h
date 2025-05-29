/*
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: Generates a 32-bit true random number.
 * Author: CompanyName
 * Create: 2018-10-31
 */

#ifndef __SOC_MDM_RANDOM_H__
#define __SOC_MDM_RANDOM_H__

#include <soc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup  hct_cipher
 * @brief  Generates a 32-bit true random number.
 *
 * @par Description:
 * Generates a 32-bit true random number.
 *
 * @attention None
 * @param  min  [IN] Type #td_u32, lower limit of the random number range.
 * @param  max  [IN] Type #td_u32, upper limit of the random number range. This function has no return value for
 * an exception. The user must ensure that the minimum value is less than the maximum value.
 *
 * @retval #td_u32 Random number.
 * @par Dependency:
 * @li soc_mdm_random.h: This file describes the secondary development APIs.
 * @see None
 * @since DW21_V100R001C00
 */
td_u32 uapi_get_random_num32(td_u32 min, td_u32 max);

#ifdef __cplusplus
}
#endif
#endif /* _SOC_MDM_RANDOM_H_ */
