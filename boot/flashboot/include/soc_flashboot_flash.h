/* *
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: flash boot interfaces
 * Author: CompanyName
 * Create: 2018-10-31
 */

/* *@defgroup hct_boot_flash  FLASHBOOT_FLASH½Ó¿Ú
 * @ingroup boot_rom
 */
#ifndef __SOC_FLASHBOOT_FLASH_H__
#define __SOC_FLASHBOOT_FLASH_H__

#include <soc_types.h>

/* *
 * @ingroup  hct_boot_flash
 * @brief   Flash initialization
 *
 * @par description:
 * initialize Flash model
 * @attention
 * @retval #0      success.
 * @retval #not 0    failed.see soc_errno.h for details
 *
 * @par Dependency:
 * <ul><li>soc_romboot_flash.h: file with statement of this interface .</li></ul>
 * @see
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_init(td_void);

/* *
 * @ingroup  hct_boot_flash
 * @brief  delete flash data
 *
 * @par description:
 * delete data in flash
 * @attention
 *
 * @param  flash_addr [IN] type #td_u32  appoint the flash offset address for earasing
 * @param  flash_erase_size [IN] type #td_u32   length of erasing byte, must be a multiple of 4K
 *
 * @retval #0      success.
 * @retval #not 0    failed.see soc_errno.h for details
 *
 * @par Dependency:
 * <ul><li>soc_romboot_flash.h: file with statement of this interface .</li></ul>
 * @see  uapi_flash_write|uapi_flash_read
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_erase(const td_u32 flash_addr, td_u32 flash_erase_size);

/* *
* @ingroup  hct_boot_flash
* @brief  write data to flash
*
* @par description:
*         write data to flash
* @attention
* @li : set do_erase to TD_TRUE only when flash is initializing. see kernel function uapi_flash_init for setting method.
* @li : support the option of do_erase which equal to TD_TRUE. In function uapi_flash_init(),
* apply space for p_spif_ctrl->p_back_up_buf and configurate compare space with read-back flash.

* @param  flash_addr         [IN] type #td_u32  offset address of flash
* @param  flash_write_size   [IN] type #td_u32   length of data for writing to flash (unit: byte)
* @param  p_flash_write_data [IN] type #td_u8*  data for writing to flash
* @param  do_erase           [IN] type #td_bool  enable the feature that erase first before write to flash.
* by default, equal to TD_FALSE, see the function for more attentions.
*
*
* @retval #0      success.
* @retval #not 0    failed.see soc_errno.h for details
*
* @par Dependency:
* <ul><li>soc_romboot_flash.h: file with statement of this interface .</li></ul>
* @see  uapi_flash_erase|uapi_flash_read
* @since DW21_V100R001C00
 */
td_u32 uapi_flash_write(td_u32 flash_addr, td_u32 flash_write_size, td_u8 *flash_write_data, td_bool do_erase);

/* *
 * @ingroup  hct_boot_flash
 * @brief  read data from flash
 *
 * @par description:
 * read data from flash
 * @attention :nothing
 *
 * @param  flash_addr [IN] type #td_u32  offset address of flash
 * @param  flash_read_size [IN] type #td_u32   length of data for reading from flash (unit: byte)
 * @param  p_flash_read_data [OUT] type #td_u8*  , using for storing the data read from flash.
 * verify the validity of the pointer, because inner interface doesn't verify the pointer.
 *
 * @retval #0      success.
 * @retval #not 0    failed.see soc_errno.h for details
 *
 * @par Dependency:
 * <ul><li>soc_romboot_flash.h: file with statement of this interface  .</li></ul>
 * @see  uapi_flash_write|uapi_flash_erase
 * @since DW21_V100R001C00
 */
td_u32 uapi_flash_read(td_u32 flash_addr, td_u32 flash_read_size, td_u8 *flash_read_data);
#endif
