/**
 * Copyright (c) CompanyNameMagicTag 2018-2019. All rights reserved.
 * Description: 1901 protocol ndm point to point function interface. \n
 * Author: CompanyName
 * Create: 2018-10-17
 */
#ifndef MDM_INC_P2P_P1901_INTERFACE_H_
#define MDM_INC_P2P_P1901_INTERFACE_H_

#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
td_bool p2p_ndm_is_in_ciu_mode(td_void);
td_bool p2p_ndm_is_in_search_net_mode(td_void);
td_void p2p_restart_p2p_search(td_u8 *oda, td_u8 mac_len);
td_void p2p_stop_active_search_timer(td_void);
#endif

#if defined(PRODUCT_CFG_PRODUCT_TYPE_STA)
td_bool p2p_sta_is_in_pushing_state(td_void);
#endif
#endif
