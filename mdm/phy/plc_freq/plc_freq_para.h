/*
 * Copyright (c) CompanyNameMagicTag 2017-2020. All rights reserved.
 * Description: Supported frequency parameters getter interfaces
 */

#ifndef __PLC_FREQ_PARA_H__
#define __PLC_FREQ_PARA_H__

/* Get frequency related parameters */
td_u32 *plc_get_freq_cfg_param(td_void);

/* Get valid frequency number */
td_u8 plc_get_freq_cfg_num(td_void);

#endif