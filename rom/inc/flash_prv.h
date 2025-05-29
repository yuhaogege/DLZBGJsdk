#ifndef __FLASH_PRV_H__
#define __FLASH_PRV_H__
#include <soc_types_rom.h>
#include <soc_mdm_types_rom.h>
#include <dw21_platform_rom.h>
#define flash_info_print(fmt, ...)

#define EXT_FLASH_OPTION_READ_WAIT_READY         0x1
#define EXT_FLASH_OPTION_WRITE_WAIT_READY        0x2
#define EXT_FLASH_OPTION_ERASE_WAIT_READY        0x4
#define EXT_FLASH_OPTION_WRITE_ENABLE_WAIT_READY 0x8

/*****************************************************************************/
#define SPI_QE_EN 0x02 /* QE Bit Enable */

#define SPI_CMD_WREN     0x06 /* Write Enable */
#define SPI_CMD_VSR_WREN 0x50 /* Write volatile SR reg ENABLE */
// -----------------------------------------------------------------------------
#define SPI_CMD_SE_4K  0x20 /* 4KB sector Erase */
#define SPI_CMD_SE_32K 0x52 /* 32KB sector Erase */
#define SPI_CMD_SE     0xD8 /* 64KB Sector Erase */
#define SPI_CMD_BE     0xC7 /* chip erase */
// -----------------------------------------------------------------------------
#define SPI_CMD_WRSR1 0x01 /* Write Status Register */
#define SPI_CMD_RDSR  0x05 /* Read Status Register */

#define SPI_CMD_WRSR2 0x31 /* Write Status Register-2 */
#define SPI_CMD_RDSR2 0x35 /* Read Status Register-2 */

#define SPI_CMD_WRSR3 0x11 /* Write Status Register-3 */
#define SPI_CMD_RDSR3 0x15 /* Read Status Register-3 */

#define SPI_CMD_RDID 0x9F /* Read Identification */
// -----------------------------------------------------------------------------
#define SPI_CMD_PP              0x02 /* Page Programming */
#define SPI_CMD_WRITE_DUAL      0xA2 /* fast program dual input */
#define SPI_CMD_WRITE_QUAD      0x32 /* fast program quad input */
#define SPI_CMD_WRITE_DUAL_ADDR 0xD2 /* Dual I/O High Performance Write */
#define SPI_CMD_WRITE_QUAD_ADDR 0x12 /* Quad I/O High Performance Write */
// -----------------------------------------------------------------------------
#define SPI_CMD_READ      0x03 /* Read Data bytes */
#define SPI_CMD_FAST_READ 0x0B /* Read Data Bytes at Higher Speed */
#define SPI_CMD_READ_DUAL 0x3B /* fast read dual output */
#define SPI_CMD_READ_QUAD 0x6B /* fast read quad output */
#define SPI_CMD_QUAD_IO   0xEB /* fast quad-IO */

#define SPI_CMD_READ_DUAL_ADDR 0xBB /* Dual I/O High Performance Read */
#define SPI_CMD_READ_QUAD_ADDR 0xEB /* Quad I/O High Performance Read */
// -----------------------------------------------------------------------------
#define SPI_CMD_SR_WIPN 0 /* Write in Progress */
#define SPI_CMD_SR_WIP  1 /* Write in Progress */
#define SPI_CMD_SR_WEL  2 /* Write Enable Latch */
// -----------------------------------------------------------------------------
#define SPI_CMD_EN4B 0xB7 /* enter to 4 bytes mode and set 4 byte bit as '1' */
#define SPI_CMD_EX4B 0xE9 /* exit 4 bytes mode and clear 4 byte bit as '0' */

#define SPI_CMD_RESET_ENABLE 0x66 /* RESET Enable */
#define SPI_CMD_RESET        0x99 /* RESET */

#define SPI_SR_BIT_WIP (1 << 0) /* Write in Progress */
#define SPI_SR_BIT_WEL (1 << 1) /* Write Enable Latch */

#define _1K (0x400)
#define _2K (0x800)

#define _4K  (0x1000)
#define _8K  (0x2000)
#define _16K (0x4000)
#define _32K (0x8000)

#define _64K  (0x10000)
#define _128K (0x20000)
#define _256K (0x40000)
#define _512K (0x80000)

#define _1M (0x100000)
#define _2M (0x200000)
#define _4M (0x400000)
#define _8M (0x800000)

#define _16M (0x1000000)
#define _32M (0x2000000)

#define _CHIP_SIZE (0x3ffff)

typedef struct
{
    td_u32 cmd : 8;    /* Command ID. For details about the value, see the reg_ins member 
                                     * of the CMD_INS register in the PLC Chip User Guide. 
                                     */
    td_u32 iftype : 3; /* Interface type. For details about the value, see the mem_if_type member of the CMD_CONFIG register
                                     * in the Power Line Carrier Communication Chip User Guide. 
                                     */
    td_u32 dummy : 3;  /* Dummy byte. For details about the value, see the dummy_byte_cnt 
                                     * member of the CMD_CONFIG register in the PLC Chip User Guide. 
                                     */
    td_u32 size : 18;  /* Erase size (meaningless for non-erase commands). 
                                     * The value _CHIP_SIZE indicates that the entire block is erased. 
                                     */
} SPI_FLASH_OPERATION_S;

typedef enum {
    EXT_SPI_FLASH_CHIP_ATTRIBUTE_IS_DEFAULT_ITEM = 0x1,     /* The flash memory is the default configuration. */
    EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_4K_ERASE = 0x2,    /* Flash memory supports 4K erasure. */
    EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_32K_ERASE = 0x4,   /* Flash memory supports 32 KB erasing. */
    EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_64K_ERASE = 0x8,   /* The flash memory supports 64 KB erasure. */
    EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_CHIP_ERASE = 0x10, /* Flash memory erases the entire flash memory. */
} EXT_SPI_FLASH_CHIP_ATTRIBUTE_E;

typedef struct
{
    td_char *chip_name;    /* Chip Name */
    td_u8 chip_id[8];      /* Chip Identification ID */
    td_u32 chip_size;      /* Indicates the total capacity of the chip, in bytes. */
    td_u16 chip_attribute; /* Chip attribute. For details, see ext_flash_protect_type_e. */
    td_u8 chip_id_len;     /* Length of the chip ID to be matched. For example, when the value is 3, 
                                            * the ID read from the chip by running the 0x9F command only needs to 
                                            *¡¡match the first 3 bytes of chip_id. 
                                            */
    td_u8 protect_type;    /* Flash protection type. For details, see ext_flash_protect_type_e. */
} EXT_SPI_FLASH_BASIC_INFO_S;

#define EXT_FLASH_SUPPORT_ERASE464CHIP (EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_4K_ERASE | EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_64K_ERASE | EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_CHIP_ERASE)
#define EXT_FLASH_SUPPORT_DEFAULT      (EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_4K_ERASE | EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_64K_ERASE | EXT_SPI_FLASH_CHIP_ATTRIBUTE_SUPPORT_CHIP_ERASE | EXT_SPI_FLASH_CHIP_ATTRIBUTE_IS_DEFAULT_ITEM)

#define PRODUCT_CFG_FLASH_BLOCK_SIZE 0x1000
/*****************************************************************************/
#define EXT_SFC300_DMA_MAX_SIZE (2048)
#define EXT_SFC300_DMA_MAX_MASK (0x7FF) /* The DMA size cannot exceed 2048 MB. */

/*****************************************************************************/
#define EXT_SFC300_REG_BUF_SIZE (256)
#define EXT_SFC300_REG_BUF_MASK (0xFF)

#define EXT_SFC300_BUS_CONFIG2       0x0204
#define EXT_SFC300_BUS_BASE_ADDR_CS1 0x0218
#define EXT_SFC300_BUS_ALIAS_ADDR    0x021C
#define EXT_SFC300_BUS_ALIAS_CS      0x0220
#define EXT_SFC300_CMD_DATABUF64     0x04FC

/***************SFC of dw11V100********************************************/
#define SFC_REG_BASE_ADDRESS    EXT_SFC_REG_BASE
#define SFC_BUFFER_BASE_ADDRESS (0x01000000)

#define SFC_REG_GLOBAL_CONFIG               0x0100
#define SFC_REG_GLOBAL_CONFIG_ADDR_MODE_4B  (1 << 2)
#define SFC_REG_TIMING                      0x0110
#define SFC_REG_GLOBAL_CONFIG_WP_ENABLE     (1 << 1)
#define SFC_TIMING_TSHSL(_n)                ((_n)&0xF)
#define SFC_TIMING_TSHWL(_n)                (((_n)&0xF) << 4)
#define SFC_TIMING_TCSS(_n)                 (((_n)&0x7) << 8)
#define SFC_TIMING_TCSH(_n)                 (((_n)&0x7) << 12)
#define SFC_TIMING_TRPD(_n)                 (((_n)&0xFFF) << 16)
#define SFC_REG_INT_RAW_STATUS              0x0120 /* Interrupt raw status register */
#define SFC_REG_INT_RAW_STATUS_DMA_DONE     (1 << 1)
#define SFC_REG_INT_STATUS                  0x0124
#define SFC_REG_INT_MASK                    0x0128
#define SFC_REG_INT_CLEAR                   0x012C
#define SFC_REG_INT_CLEAR_DMA_DONE          (1 << 1)
#define SFC_REG_VERSION                     0x01F8
#define SFC_REG_VERSION_SEL                 0x01FC
#define SFC_REG_BUS_CONFIG1                 0x0200
#define SFC_BUS_CONFIG1_WR_INS(_n)          ((_n & 0xFF) << 22)
#define SFC_BUS_CONFIG1_RD_INS(_n)          ((_n & 0xFF) << 8)
#define SFC_BUS_CONFIG1_RD_PREFETCH_CNT(_n) ((_n & 0x3) << 6)
#define SFC_BUS_CONFIG1_RD_DUMMY_BYTES(_n)  ((_n & 0x7) << 3)
#define SFC_BUS_CONFIG1_RD_MEM_IF_TYPE(_n)  (_n & 0x7)
#define SFC_BUS_CONFIG1_RD_ENABLE           ((td_u32)(1 << 31))

#define SFC_REG_BUS_FLASH_SIZE    0x0210
#define SFC_REG_BUS_BASE_ADDR_CS0 0x0214
#define SFC_REG_BUS_BASE_ADDR_CS1 0x0218

#define SFC_REG_BUS_DMA_CTRL           0X0240
#define SFC_BUS_DMA_CTRL_START         (1 << 0)
#define SFC_BUS_DMA_CTRL_READ(_dir)    ((_dir) << 1)
#define SFC_BUS_DMA_CTRL_CS(_cs)       (((_cs)&0x01) << 4)
#define SFC_REG_BUS_DMA_MEM_SADDR      0X0244 /* DMA DDR start address R */
#define SFC_REG_BUS_DMA_FLASH_SADDR    0X0248
#define SFC_REG_BUS_DMA_LEN            0x024C
#define SFC_REG_BUS_DMA_AHB_CTRL       0X0250
#define SFC_BUS_DMA_AHB_CTRL_INCR4_EN  (1 << 0)
#define SFC_BUS_DMA_AHB_CTRL_INCR8_EN  (1 << 1)
#define SFC_BUS_DMA_AHB_CTRL_INCR16_EN (1 << 2)

#define SFC_REG_CMD_CONFIG                0x0300 /* Command operation mode configuration register */
#define SFC_CMD_CONFIG_MEM_IF_TYPE(_n)    (((_n)&0x07) << 17)
#define SFC_CMD_CONFIG_DATA_CNT(_n)       (((_n - 1) & 0xFF) << 9)
#define SFC_CMD_CONFIG_RW                 (1 << 8)
#define SFC_CMD_CONFIG_DATA_EN            (1 << 7)
#define SFC_CMD_CONFIG_DUMMY_BYTE_CNT(_n) (((_n)&0x07) << 4)
#define SFC_CMD_CONFIG_ADDR_EN            (1 << 3)
#define SFC_CMD_CONFIG_SEL_CS             (0x01 << 1)
#define SFC_CMD_CONFIG_START              (1 << 0)
#define SFC_REG_CMD_INS                   0x0308 /* Command operation mode instruction register */

#define SFC_REG_CMD_ADDR     0x030C
#define SFC_CMD_ADDR_MASK    (0x3FFFFFFF)
#define SFC_REG_CMD_DATABUF1 0x0400

#define SPI_SR3_DRV_MASK (0x3)

#define SFC_ERASE_OPT_MAX_NUM 4

typedef enum {
    SPI_SR3_DRV_100PCT = 0,
    SPI_SR3_DRV_75PCT,
    SPI_SR3_DRV_50PCT,
    SPI_SR3_DRV_25PCT,
    SPI_SR3_DRV_MAX,
} EXT_FLASH_DRV_STRENGTH_E;

#define SFC_CMD_WRITE (0 << 8)
#define SFC_CMD_READ  (1 << 8)

typedef enum {
    EXT_FLASH_CHECK_PARAM_OPT_READ,
    EXT_FLASH_CHECK_PARAM_OPT_WRITE,
    EXT_FLASH_CHECK_PARAM_OPT_ERASE,
} EXT_FLASH_CHECK_PARAM_OPT_E;

typedef struct EXT_SPI_FLASH_CTRL_S {
    td_u32 bInit;                         /* Whether the flash global control structure is initialized */
    EXT_SPI_FLASH_BASIC_INFO_S basic_info; /* Basic information about the flash driver in use */
    SPI_FLASH_OPERATION_S opt_read;       /* Information about the read command of the current flash driver */
    SPI_FLASH_OPERATION_S opt_write;      /* Information about the write command of the current flash driver */
    /* Indicates the erase command of the flash driver in use. Multiple erase commands are supported.
       * Select the corresponding erase command based on the address and size of the erase block. For example, 
       * perform full-chip erasing, 64K erasing, and 4K erasing in sequence. */
    SPI_FLASH_OPERATION_S array_opt_erase[SFC_ERASE_OPT_MAX_NUM];

    td_u32 erase_size;        /* Minimum number of erase blocks */
    td_u32 dma_ram_size;      /* DMA buffer(p_dma_ram_buffer) size£¬Block size of a single DMA read/write operation */
    td_u8 *p_dma_ram_buffer;  /* DMA buffer Space */
    td_u8 *p_back_up_buf;     /* If the flash write interface needs to support the setting of do_erase to 
                                                 * TD_TRUE, the space needs to be applied for in the initialization process. 
                                                 * The size of this space is specified by erase_size. This space is used to 
                                                 * read the contents of the sector in advance for backup. --> Erase the sector.
                                                 * --> Modify the data in the memory. --> Write the data back to the flash memory. 
                                                 */
    td_u32(*p_read)(struct EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 read_size,
           td_void *ram_addr, td_bool b_is_crash);   /* Bottom-layer read hook function */
    td_u32(*p_write)(struct EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 write_size,
           td_void *ram_addr, td_bool b_is_crash); /* Bottom-layer write hook function */
    td_u32(*p_erase)(struct EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 erase_size,
           td_bool b_is_crash);                    /* Bottom rub hook subfunction */
    td_u32 mutex_handle;  /* Interlocking lock */
    td_pvoid pUsrData;    /* Indicates the user data, which is used for extension. */
} EXT_SPI_FLASH_CTRL_S;

/*****************************************************************************/
#define ext_sfc_read(_reg) \
    reg_read_val((uintptr_t)SFC_REG_BASE_ADDRESS + (_reg))

#define ext_sfc_write(_reg, _value) \
    reg_write32((uintptr_t)SFC_REG_BASE_ADDRESS + (_reg), (_value))

EXT_EXTERN td_u32 spi_flash_read_chip_id(td_u8 chip_id[8]);
EXT_EXTERN td_u32 spi_flash_configure_driver_strength(EXT_FLASH_DRV_STRENGTH_E drv_strength);
/* spi_flash_set_protect The value depends on the specific chip. If the value is different from that in the table, 
 * you need to customize the value. 
 */
__isr EXT_EXTERN td_u32 spi_flash_set_protect(td_u8 cmp_bp, td_bool is_volatile);

EXT_EXTERN td_u32 spif_map_chipsize(td_u32 ulChipSize);
EXT_EXTERN td_u32 spif_dma_read(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 read_size,
                               td_void *ram_addr, td_bool b_is_crash);
EXT_EXTERN td_u32 spif_reg_erase(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 erase_size,
                                td_bool b_is_crash);
EXT_EXTERN td_u32 spif_dma_write(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u32 write_size,
                                td_void *ram_addr, td_bool b_is_crash);
EXT_EXTERN td_u32 spi_flash_basic_info_probe(EXT_INOUT EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, EXT_OUT td_u8 chip_id[8],
                                            EXT_SPI_FLASH_BASIC_INFO_S *p_spi_info_tbl, td_u32 tbl_size);
EXT_EXTERN td_u32 flash_write_prv(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 flash_addr, td_u8 *p_ram_addr,
                                 td_u32 size, td_bool do_erase);
EXT_EXTERN td_u32 flash_erase_prv(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, EXT_IN td_u32 flash_addr, EXT_IN td_u32 size);
EXT_EXTERN td_u32 flash_read_prv(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, EXT_IN td_u32 flash_addr, EXT_OUT td_u8 *data,
                                EXT_IN td_u32 size);
EXT_EXTERN td_u32 spi_flash_read_reg(td_u8 cmd, td_u8 *p_data, td_u8 data_len);
EXT_EXTERN td_u32 spi_flash_write_reg(td_u8 cmd, td_u8 *p_data, td_u8 data_len);
EXT_EXTERN td_void sfc_sys_init(td_void);
EXT_EXTERN td_void sfc_sys_exit(td_void);
EXT_EXTERN td_u32 sfc_check_para(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u32 addr, td_u32 size,
                                EXT_FLASH_CHECK_PARAM_OPT_E opt);
EXT_EXTERN td_u32 flash_ioctl(EXT_SPI_FLASH_CTRL_S *p_spif_ctrl, td_u16 cmd, td_void *data);
EXT_EXTERN td_u32 spi_flash_enable_quad_mode(td_void);
EXT_EXTERN td_u32 spif_wait_config_start(td_void);
EXT_EXTERN td_u32 spif_write_enable(td_bool b_is_crash);

EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_standard_read;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_fast_read;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_fast_dual_out_read;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_fast_quad_out_read;

EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_page_program_write;

EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_sector_erase;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_32KBLK_erase;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_64KBLK_erase;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_chip_erase1;
EXT_EXTERN EXT_CONST SPI_FLASH_OPERATION_S g_spi_opt_chip_erase2;
EXT_EXTERN EXT_CONST EXT_SPI_FLASH_BASIC_INFO_S g_flash_default_info_tbl[];

#endif /* SFC11H */
