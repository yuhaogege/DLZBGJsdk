/*
 * Copyright (c) CompanyNameMagicTag 2019-2019. All rights reserved.
 * Description: soc_mdm.
 * Author: CompanyName
 * Create: 2019-10-31
 */
/* @defgroup osa  OS APIs */
/* @defgroup drivers Driver APIs */
/* @defgroup system  System management APIs */
/* @defgroup phy  PLC PHY layer APIs */
/* @defgroup plc  PLC channel APIs */
/* @defgroup mid  MID APIs */
/* @defgroup hct_secure  Secure C library APIs */

/* @defgroup hct_mdm MAC layer APIs */
/* @ingroup network */

#ifndef __SOC_MDM_H__
#define __SOC_MDM_H__
#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_ft_nv.h>
#include <soc_sal_nv.h>
#include <dw21_platform.h>
#ifndef PRODUCT_CFG_DBK /* Enabling macro of the DBK tool */
#include <soc_mdm_crash.h>
#include <soc_mdm_mac.h>
#include <soc_mdm_ndm.h>
#include <soc_mdm_uart.h>
#include <soc_partition_table.h>
#include <soc_mdm_diag.h>
#include <soc_mdm_nv.h>
#include <soc_mdm_upg.h>
#include <soc_timer.h>
#include <soc_mdm_dms.h>
#include <soc_mdm_phy.h>
#include <soc_mdm_flash.h>
#include <soc_mdm_sys.h>
#include <soc_mdm_ver.h>
#include <soc_mdm_init.h>
#include <soc_mid.h>
#include <soc_mdm_hrtimer.h>
#include <soc_mdm_task.h>
#include <soc_mdm_i2c.h>
#include <soc_mdm_time.h>
#include <soc_mdm_io.h>
#include <soc_mdm_timecnt.h>
#include <soc_mdm_isr.h>
#include <soc_mdm_timer.h>
#include <soc_mdm_led.h>
#include <soc_list.h>
#include <soc_mdm_mem.h>
#include <soc_mdm_watchdog.h>
#include <soc_mdm_cipher.h>
#include <soc_mdm_msg.h>
#include <soc_stdlib.h>
#include <soc_mdm_clock.h>
#include <soc_mdm_mux.h>
#include <soc_mdm_crc.h>
#include <soc_mdm_os_stat.h>
#include <soc_mdm_event.h>
#include <soc_mdm_sem.h>
#include <soc_mdm_spi.h>
#include <soc_mdm_random.h>
#include <soc_mdm_tsensor.h>
#endif
#endif  /* __SOC_MDM_H__ */

