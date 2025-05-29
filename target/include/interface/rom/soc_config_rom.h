/**
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: ROM config code.
 */
#ifndef __SOC_CONFIG_ROM_H__
#define __SOC_CONFIG_ROM_H__

// *****************************************************************************
// BSP: [300, 400)
#define EXT_MOD_ID_ROM (311) // 0x137

#define EXT_MOD_ID_BSP_MIN 200
#define EXT_MOD_ID_BSP_MAX 399
#define EXT_MOD_ID_MAC_MIN 500
#define EXT_MOD_ID_MAC_MAX 599
#define EXT_MOD_ID_APP_MIN 800
#define EXT_MOD_ID_APP_MAX 899

/* ROM sub-ID */
// [0x0,0x10)
#define EXT_MOD_ID_AES_KEY_LEN (0x0)
#define EXT_MOD_ID_AES_NODES (0x1)

#define EXT_MOD_ID_HASH_HMAC (0x4)
#define EXT_MOD_ID_HASH_ALL (0x5) /* Used in non-segmented HASH */
#define EXT_MOD_ID_HASH_SEGMENTATION (0x6)
#define EXT_MOD_ID_HASH_NODES (0x7)

#define EXT_MOD_ID_RSA_KEY (0x9)
#define EXT_MOD_ID_RSA_PKCS1 (0xa)
#define EXT_MOD_ID_RSA_PKCS1_PSS (0xb)
#define EXT_MOD_ID_ALG_SRP (0xc)

// *****************************************************************************
// *****************************************************************************
// Global static settings
// *****************************************************************************
#define EXT_MILLISECOND_PER_TICK (10) // 10ms/tick

// *****************************************************************************
#endif // __SOC_CONFIG_ROM_H__
