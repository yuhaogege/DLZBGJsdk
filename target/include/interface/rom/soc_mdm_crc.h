/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: hct_crc  Cyclic redundancy check (CRC).
 */

#ifndef __SOC_MDM_CRC_H__
#define __SOC_MDM_CRC_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>

/**
 * @ingroup  hct_crc
 * @brief  Generates a 16-bit CRC value.
 *
 * @par Description:
 * Generates a 16-bit CRC value. The polynomial is as follows: x16 + x12 + x5 + 1 (0x1021)
 *
 * @attention The input argument needs to be checked by the user, and the buffer cannot be empty.
 * @param  crc_start         [IN] Type #td_u16 The original initial value is 0. Piecewise calculation is used. The
 * previous calculation result is transferred each time.
 * @param  buf               [IN] Type #td_u8* Pointer to the checked buffer
 * @param  len               [IN] Type #td_u32 Length of the checked buffer (unit: byte)
 *
 * @retval Type #td_u16   CRC value
 *
 * @par Dependency:
 * @li soc_mdm_crc.h: Describes CRC APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u16 uapi_crc16(td_u16 crc_start, EXT_CONST td_u8 *buf, td_u32 len);

/**
 * @ingroup  hct_crc
 * @brief  Generates a 32-bit CRC value.
 *
 * @par Description:
 * Generates a 32-bit CRC value. The 32-bit CRC algorithm complies with the IEEE 802.3 CRC-32 standard (0x04C11DB7).
 *
 * @attention The input argument needs to be checked by the user, and the buffer cannot be empty.
 * @param  crc_start         [IN] Type #td_u32 Initial CRC value. The original initial value is 0. Piecewise calculation
 * is used. The previous calculation result is transferred each time.
 * @param  buf               [IN] Type #td_u8* Pointer to the checked buffer
 * @param  len               [IN] Type #td_u32 Length of the checked buffer (unit: byte)
 *
 * @retval Type #td_u32   CRC value
 * @par Dependency:
 * @li soc_mdm_crc.h: Describes CRC APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_crc32(td_u32 crc_start, EXT_CONST td_u8 *buf, td_u32 len);

/**
 * @ingroup  hct_crc
 * @brief  Generates a 32-bit CRC value without ones complement.
 *
 * @par Description:
 * Generates a 32-bit CRC value without ones complement. The 32-bit CRC algorithm complies with the 1901.1 CRC-32
 * standard (0x04C11DB7).
 *
 * @attention The input argument needs to be checked by the user, and the buffer cannot be empty.
 * @param  crc_start         [IN] Type #td_u32 Initial CRC value. The original initial value is 0. Piecewise calculation
 * is used. The previous calculation result is transferred each time.
 * @param  buf               [IN] Type #td_u8* Pointer to the checked buffer
 * @param  len               [IN] Type #td_u32 Length of the checked buffer (unit: byte)
 *
 * @retval Type #td_u32   CRC value
 * @par Dependency:
 * @li soc_mdm_crc.h: Describes CRC APIs.
 * @see  None
 * @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_crc32_no_comp(td_u32 crc_start, EXT_CONST td_u8 *buf, td_u32 len);

#endif
