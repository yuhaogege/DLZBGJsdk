/**@defgroup boot_rom  BOOT interface */
/**@defgroup hct_boot_api  ROMBOOT interface
 * @ingroup boot_rom
 */

#ifndef SOC_BOOT_ROM_H
#define SOC_BOOT_ROM_H
#include <soc_types.h>
#include <soc_boot_err.h>
#include <dw21_platform.h>

/** 
 * @ingroup  hct_boot_api
 * Set memory size to 256KB. The value cannot be changed.
 * It's used for judging boundary when memory read OR write operation performed in program.
 */
#define SRAM_LENGTH (0x40000)
/** 
 * @ingroup  hct_boot_api
 * Set memory start address. The value cannot be changed.
 * It's used for judging boundary when memory read OR write operation performed in program.
 */
#define SRAM_START_ADDR (0x02000000)
/** 
 * @ingroup  hct_boot_api
 * Define the ascii code of Ctrl+C. Currently, this command is used to interrupt the command input;
 * It is also used to confiure the characters that enter the command line.
 */
#define CHAR_CTRL_C (0x03)

/** 
 * @ingroup  hct_boot_api
 * DW21SV100¡£
 */
#define EXT_CHIP_TYPE_21SV 1
/** 
 * @ingroup  hct_boot_api
 * DW21EV100¡£
 */
#define EXT_CHIP_TYPE_21EV 0

/** 
 * @ingroup  hct_boot_api
 * It is used to configure the variable paremeters in the boot. Generally, configure parameters during system startup.
 */
typedef struct soft_value {
    td_u32 cmd_burn_default_sram_addr;  /**< by default, the burn command uses memory address */
    td_u32 cmd_burn_default_flash_addr; /**< by default, the burn command is used to burn flash address */
    td_u32 cmd_burn_default_flash_len;  /**< by default, the burn command is used to burn the length of flash memory */
    td_u32 cmd_loady_default_sram_addr; /**< by default, the loady command downlod memory address.
                                        * The space reserved at least equal to the file size. */
    td_u32 uart_interrupt_type;         /**< uart interrupt frame type */
    td_u8 *malloc_start_addr;           /**< start memory address of malloc space */
    td_u8 *malloc_end_addr;             /**< end memory address of malloc space */
    td_void *flash_cmd_start;           /**< start memory address of flash expand command */
    td_void *flash_cmd_end;             /**< end memory address of flash expand command */
    td_char interrupt_char;             /**< interrupt character */
    td_char prompt[14];                 /**< shell prompt */
} SOFT_VALUE;
EXT_EXTERN SOFT_VALUE g_st_softvalue;

/****************************************software maintenance and commissioning*********************************/
/**
 * @ingroup  hct_boot_api
 * 
 * Works with set_start_status to set breakpoint function and record the start track function.
 * Users cannot change or add.
 */

typedef enum {
    EXT_BOOT_ENTER_C = 0x5A5A5A01,        /**< from the assembly to c code */
    EXT_BOOT_UART_INIT,                   /**< UART initialization completed */
    EXT_BOOT_WAIT_UART_INTERRUPT,         /**< wait for the uart interrupt to enter the shell command line phase */
    EXT_BOOT_OMIT_UART_AUTH,              /**< set EXT_EFUSE_SDC_RW_ID to 0 after recieved shell interrupt character.
                                          * Command line security authentication is not required, and enter the command line directly */
    EXT_BOOT_UART_AUTH_SUCCESS,           /**< BOOT security authentication is passed, enter tne command line */
    EXT_BOOT_UART_SEND_RAND,              /**< Basic authentication process 1: the board sends a random number to Burn */
    EXT_BOOT_UART_RECV_MSG,               /**< Basic authentication process 2: the board recieved the message replied from Burn */
    EXT_BOOT_UART_CHANLLANGE,             /**< Basic authentication failed. Enter the challenge process */
    EXT_BOOT_ENTER_SHELL,                 /**< enter shell */
    EXT_BOOT_START_SECURITY_VERIFY,       /**< startup flash_boot security authentication */
    EXT_BOOT_BOOT_RESV,                   /**< reserved */
    EXT_BOOT_BOOT_ROOT_VERIFY,            /**< If EXT_EFUSE_SECURE_BOOT_RW_ID is 1, enable the flash_boot security authentication */
    EXT_BOOT_BOOT_SUBKEY_VERIFY,          /**< root key authentication passed, start the level-2 key authentication */
    EXT_BOOT_BOOT_MAINTENANCE_KEY_VERIFY, /**< level-2 key authentication passed, start the maintenance process verification. The maintenance is not mandatory. */
    EXT_BOOT_BOOT_LOADER_VERIFY,          /**< level-2 key and maintenance key authentication passed. start verify the bootloader content. */
    EXT_BOOT_OMIT_SECURITY_VERIFY,        /**< If EXT_EFUSE_SECURE_BOOT_RW_ID is 0, the boorloader content is not started safely */
    EXT_BOOT_LOADER_START,                /**< flashboot Non-compressed storage, jumping to boot flashboot startup */
    EXT_BOOT_UNLZMA_START,                /**< start decompress the flashboot. The size of the decompressed file is less than 64K */
    EXT_BOOT_LOADER_UNLZMA_START,         /**< complete decompress flashboot, startup flashboot */
    EXT_BOOT_SOFT_REBOOT,                 /**< flash boot security authentication failed. Restart the board */
    EXT_BOOT_IDX_MAX,                     /**< Maximum startup status */
} ext_boot_start_status_e;

/* when this bit is equal to the ID value, enter the infinite loop waits for debugging */
#define STATUS_DEBUG_MEM_ADDR (EXT_SYSCTRL_REG_BASE + SC_GEN_REG0) /* 0x40030040 */
/**
* @ingroup  hct_boot_api
* @brief  Users can use this to record romboot start track and use the breakpoint function. Secondary development is not supported.
*
* @par description:
*         @li record start track: when the system starts, record the start track to the member en_start_status of EXT_BOOT_DFX_STRU according to the bit. EXT_BOOT_ENTER_C indicates the 1st bit, and so on.
*         @li breakpoint function: before debugging, set the general register STATUS_DEBUG_MEM_ADDR to the status member value of EXT_BOOT_START_STATUS_E. When program runs to this status, the system enter the infinite loop waits for debugging.
* @attention This interface only records the rom status. Users cannot invoke this interface.
*
* @param  en_status [IN] type #ext_boot_start_status_e  key node status enumeration list
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  nothing
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void set_start_status(ext_boot_start_status_e en_status);

/**
* @ingroup  hct_boot_api
* @get chip type
*
* @par description:
*           get chip type
* @attention 
*
* @param  nothing
*
* @retval # EXT_CHIP_TYPE_21SV or EXT_CHIP_TYPE_21EV  chip type.
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  EXT_CHIP_TYPE_21SV and EXT_CHIP_TYPE_21EV
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_get_hw_chip_type(td_void);

typedef struct boot_dfx {
    td_u32 heap_malloc_size;   /**< total size of the heap space already applied */
    td_u32 heap_free_size;     /**< total size of remain heap space */
    td_u32 heap_size;          /**< total size of heap space */
    uintptr_t heap_start_addr; /**< start address of heap space */
    uintptr_t heap_end_addr;   /**< end address of heap space */
    td_u32 en_start_status;    /**< start status maintenance, works with set_start_status function */
} EXT_BOOT_DFX_STRU;
EXT_EXTERN EXT_BOOT_DFX_STRU g_st_dfx;

/* register read/write interface */
#define UAPI_REG_WRITE(addr, val) (*(volatile td_u32*)(uintptr_t)(addr) = (val))
#define UAPI_REG_READ(addr, val)  ((val) = *(volatile td_u32*)(uintptr_t)(addr))
#define UAPI_REG_READ32(addr)     (*(volatile td_u32*)(uintptr_t)(addr))

/* type definition */
typedef td_u32 size_t;
typedef td_s32 errno_t;

/***************************************system interface**************************************/
EXT_EXTERN td_s32 memcmp(const td_void *cs, const td_void *ct, size_t count);
EXT_EXTERN td_s32 strcmp(const td_char *cs, const td_char *ct);
EXT_EXTERN td_s32 strncmp(const td_char *cs, const td_char *ct, size_t count);
EXT_EXTERN size_t strlen(const td_char *s);
EXT_EXTERN td_char *strchr(const td_char *s, td_s32 c);

errno_t memcpy_s(td_void *dest, size_t destMax, const td_void *src, size_t count);
errno_t memset_s(td_void *dest, size_t destMax, td_s32 c, size_t count);
errno_t strcpy_s(td_char *strDest, size_t destMax, const td_char *strSrc);
/***************************************delay interface**************************************/

/**
* @ingroup  hct_boot_api
* @brief  ms delay function. The software calculates the delay time based on the CPU clock.
*
* @par description:
*           ms delay function.
* @attention   nothing
*
* @param  msecs [IN] type #td_u32   delay time, unit: ms
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  udelay
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void mdelay(td_u32 msecs);

/**
* @ingroup  hct_boot_api
* @brief  us delay function. The software calculates the delay time based on the CPU clock.
*
* @par description:
*         us delay function.
* @attention   nothing
*
* @param  usecs [IN] type #td_u32   delay time, unit: us
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  mdelay
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void udelay(td_u32 usecs);

/*****************************************************UART interface**************************************/

/**
 * @ingroup hct_boot_api
 * uart number
 */
typedef enum {
    UART0 = 0, /**< uart0 */
    UART1 = 1, /**< uart1 */
} ext_uart_e;

/**
 * @ingroup hct_boot_api
 * uart parameters structure
 */
typedef struct
{
    td_u32 baudrate;           /**< baud rate, for example:115200 */
    td_uchar databit;          /**< 5; 6; 7; 8 */
    td_uchar stopbit;          /**< 0:stop_bit 1;  1: when data_bit is 5, stop_bit is 1.5; else stop_bit is 2. */
    td_uchar parity;           /**< 0: None; 1:Odd; 2:Even */
    td_uchar flowctrl;         /**< 0: None; 1: Hardware_flowctrl. */
    td_uchar fifoline_full;    /**< 0:1Byte in fifo; 1:1/4 fifo; 2:1/2 fifo; 3:2Byte to full */
    td_uchar fifoline_empty;   /**< 0:empty in fifo; 1:2Byte in fifo; 2:1/4 fifo; 3:1/2 fifo */
    td_uchar baudrate_compute; /**< 0: compare to 0.5; 1:as integer; 2: if note integer, then add 1. */
    td_uchar flow_gpio;        /**< 0: not ctrl gpio; 1£ºGPIO5.6: rts_1; cts_1 :func2; 2£ºGPIO11.12: rts_0; cts_0: func1; 
                                         3£ºGPIO11.12: rts_1; cts_1: func4; 4£ºGPIO19.20: rts_1; cts_1: func3; */
} uart_param_stru;

/** 
* @ingroup  hct_boot_api
* Default serial port full threshold configuration. Invoke the serial_init function to configure the fifoline_full parameter in the input parameter UART_PARAM_STRU.
* 
* @li The value has been used by romboot and cannot be modified.
* @li In the flashboot, you can modify the configuration by redefining the macro.
 */
#define DEFAULT_FIFOLINE_FULL (2)
/** 
* @ingroup  hct_boot_api
* Default serial port empty threshold configuration. Invoke the serial_init function to configure the fifoline_empty parameter in the input parameter UART_PARAM_STRU.
* 
* @li The value has been used by romboot and cannot be modified.
* @li In the flashboot, you can modify the configuration by redefining the macro.
 */
#define DEFAULT_FIFOLINE_EMPTY (0)

/**
 * @ingroup hct_boot_api
 * print error code type which cannot be modified or added.
 */
typedef enum {
    EXT_PRINT_ERRNO_UART_AUTH_INIT = 0x3600,          /* failed to initialize the UART security authentication */
    EXT_PRINT_ERRNO_UART_WAIT_USR_INTER = 0x3601,     /**< not support currently */
    EXT_PRINT_ERRNO_UART_RANDOM_SEND_ERR = 0x3602,    /**< failed to send uart security authentication random number */
    EXT_PRINT_ERRNO_UART_RANDOM_RSPONSE_ERR = 0x3603, /**< failed to recieve uart security authentication random number reply frame */
    EXT_PRINT_ERRNO_UART_AUTH_ERR = 0x3604,           /**< serial port security authentication failed */
    EXT_PRINT_ERRNO_BOOT_HEAD_ERR = 0x3605,           /**< flashboot head verify failed */
    EXT_PRINT_ERRNO_BOOT_VERIFY_EFUSE_ERR = 0x3606,   /**< failed to read EXT_EFUSE_SECURE_BOOT_RW_ID */
    EXT_PRINT_ERRNO_BOOT_ROOT_KEY_LEN_ERR = 0x3607,   /**< flashboot head length is incorrect */
    EXT_PRINT_ERRNO_BOOT_ROOT_KEY_ERR = 0x3608,       /**< failed to authenticate the root key */
    EXT_PRINT_ERRNO_BOOT_SUB_KEY_LEN_ERR = 0x3609,    /**< level-2 key length is incorrect */
    EXT_PRINT_ERRNO_BOOT_SUB_KEY_ERR = 0x360A,        /**< failed to authenticate the level-2 key */
    EXT_PRINT_ERRNO_BOOT_MAIN_KEY_ERR = 0x360B,       /**< failed to authenticate the maintenace key */
    EXT_PRINT_ERRNO_BOOT_LOADER_VERIFY_ERR = 0x360C,  /**< flashboot verify failed */
    EXT_PRINT_ERRNO_BOOT_READ_VER_EFUSE_ERR = 0x360D, /**< failed to read EXT_EFUSE_INSECURE_VERSION_RW_ID */
    EXT_PRINT_ERRNO_BOOT_VER_ERR = 0x360E,            /**< failed to verify the BOOT version number. The R&D version and production version are incorrect */
    EXT_PRINT_ERRNO_BOOT_UNLZMA_ERR = 0x360F,         /**< flashboot decompressed failed */
    EXT_PRINT_ERRNO_MALLOC_EXAUST_ERR = 0x3610,       /**< the heap memory is insufficient */
    EXT_PRINT_ERRNO_MALLOC_STRUCT_ERR = 0x3611,       /**< heao memory management structure is damaged */
    EXT_PRINT_ERRNO_IDX_MAX,                          /**< maximum error code type */
} ext_print_errno_e;

/**
* @ingroup  hct_boot_api
* @brief  The output error code is forcibly printed in the romboot. The ext_print_errno_e definition cannot be modified.
*
* @par description:
*         forcibly print the output error code: error code is forcibly printed when an exception occurs during romboot startup,it help users locate errors.
* @attention   nothing 
*
* @param  en_print_errno [IN] type #ext_print_errno_e  error code
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  boot_puthex|boot_msg0|boot_msg1|boot_msg2|boot_msg4
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_put_errno(ext_print_errno_e en_print_errno);

/**
* @ingroup  hct_boot_api
* @brief  print output hexadecimal
*
* @par description:
* print output hexadecimal.
* @attention    nothing
*
* @param  h [IN] type #td_u32 value to be ouput
* @param  print0 [IN] type #td_bool TD_TRUE: during printing add 0 to 8 bytes. TD_FALSE: during printing, do not add bytes.
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  boot_put_errno|boot_msg0|boot_msg1|boot_msg2|boot_msg4
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_puthex(td_u32 h, td_bool print0);

/**
* @ingroup  hct_boot_api
* @brief  print output infomation. Only print character string
*
* @par description:
*         print output infomation. Only print character string
* @attention   nothing 
*
* @param  s [IN] type #td_char*  character string to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  boot_put_errno|boot_puthex|boot_msg1|boot_msg2|boot_msg4
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_msg0(const td_char *s);

/**
* @ingroup  hct_boot_api
* @brief  print output infomation. Print character string and a value
*
* @par description:
*         print output infomation. Print character string and a value
* @attention   nothing 
*
* @param  s [IN] type #td_char*  character string to be output
* @param  h [IN] type #td_u32  value in hexadecimal to be output
*
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  boot_put_errno|boot_puthex|boot_msg0|boot_msg2|boot_msg4
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_msg1(const td_char *s, td_u32 h);

/**
* @ingroup  hct_boot_api
* @brief  print output infomation. Print character string and two values
*
* @par description:
*         print output infomation. Print character string and two values
* @attention   nothing 
*
* @param  s [IN] type #td_char*  character string to be output
* @param  h1 [IN] type #td_u32  the first value in hexadecimal to be output
* @param  h2 [IN] type #td_u32  the second value in hexadecimal to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  boot_put_errno|boot_puthex|boot_msg0|boot_msg1|boot_msg4
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_msg2(const td_char *s, td_u32 h1, td_u32 h2);

/**
* @ingroup  hct_boot_api
* @brief  print output infomation. Print character string and four values
*
* @par description:
* print output infomation. Print character string and four values
* @attention   nothing 
*
* @param  s [IN] type #td_char*  character string to be output
* @param  h1 [IN] type #td_u32  the 1st value in hexadecimal to be output
* @param  h2 [IN] type #td_u32  the 2nd value in hexadecimal to be output
* @param  h3 [IN] type #td_u32  the 3rd value in hexadecimal to be output
* @param  h4 [IN] type #td_u32  the 4th value in hexadecimal to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see boot_put_errno|boot_puthex|boot_msg0|boot_msg1|boot_msg2 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_msg4(const td_char *s, td_u32 h1, td_u32 h2, td_u32 h3, td_u32 h4);

/**
* @ingroup  hct_boot_api
* @brief  forbid serial port output
*
* @par description:
* forbid serial port output: The serial port is not allowed output debugging information, but the BOOT_PUT_ERRNO output.
* @attention   nothing 
* @param nothing
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  serial_cancel_mute
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_set_mute(td_void);

/**
* @ingroup  hct_boot_api
* @brief  cancel forbidden of serial port output
*
* @par description:
* cancel forbidden of serial port output: The serial port is allowed output debugging information.
* @attention nothing
* @param nothing
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  serial_set_mute
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_cancel_mute(td_void);

/**
* @ingroup  hct_boot_api
* @brief  serial port initialization
*
* @par description:
* serial port initialization.
* @attention nothing
*
* @param  uart_num [IN] type #ext_uart_e  serial port number
* @param  default_uart_param [IN] type #uart_param_stru  serial port parameter
*
* @retval #BOOT_FAILURE failure
* @retval #BOOT_SUCCESS success
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_putc|serial_puts|serial_put_buf|serial_getc|serial_tstc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 serial_init(ext_uart_e uart_num, uart_param_stru default_uart_param);

/**
* @ingroup  hct_boot_api
* @brief  the serial port outputs a single character
*
* @par description:
* the serial port outputs a single character.
* @attention   nothing 
*
* @param  c [IN] type #td_char  the character to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_puts|serial_put_buf|serial_getc|serial_tstc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_putc(const td_char c);

/**
* @ingroup  hct_boot_api
* @brief  the serial port outputs character string
*
* @par description:
* the serial port outputs character string
* @attention   nothing 
*
* @param  s [IN] type #td_char*  the character string to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_put_buf|serial_getc|serial_tstc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_puts(const td_char *s);

/**
* @ingroup  hct_boot_api
* @brief  character buffer of serial port output, specifying the buffer length
*
* @par description:
*         character buffer of serial port output, specifying the buffer length
* @attention  
*         different from serial_puts() function£ºserial_puts function is used for character string and the output is stopped after '\0'. 
*                                                This interface is used for character buffer of a specified length and is not affected by '\0'.
*
* @param  buffer [IN] type #td_char*  the character string to be output
* @param  length [IN] type #td_s32   the character string length to be output
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_puts|serial_getc|serial_tstc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_put_buf(const td_char *buffer, td_s32 length);

/**
* @ingroup  hct_boot_api
* @brief  read a single character form serial port
*
* @par description:
*         read a single character form serial port
* @attention  
*         This function is a blocked interface. If the serial port doesn't receive any character, it waits for a long time.
* @param  nothing
*
* @retval  #character    the character read from serial port
* @retval  #-1    error
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_puts|serial_put_buf|serial_tstc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 serial_getc(td_void);

/**
* @ingroup  hct_boot_api
* @brief  check whether the serial port has character to be received
*
* @par description:
*         check whether the serial port has character to be received
* @attention  nothing
*
* @param  nothing
*
* @retval #0     there is no data on the serial port
* @retval #1     there is data on the serial port
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_puts|serial_put_buf|serial_getc|serial_puthex|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 serial_tstc(td_void);

/**
* @ingroup  hct_boot_api
* @brief  output a hexadecimal number to the serial port.
*
* @par description:
*         output a hexadecimal number to the serial port.
* @attention   nothing 
*
* @param  h [IN] type #td_u32  a hexadecimal number to be output
* @param  print_all [IN] type #td_bool  Indicates whether to add 0 before printing, 1 indicates add and 0 indicates no. The value is 4-byte-aligned.
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_puts|serial_put_buf|serial_getc|serial_tstc|serial_getc_timeout 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void serial_puthex(td_u32 h, td_bool print_all);

/**
* @ingroup  hct_boot_api
* @brief  read a character from the serial port. Ends if does not read after the specified time.
*
* @par description:
*         read a character from the serial port. Ends if does not read after the specified time.
* @attention   nothing 
*
* @param  timeout_us [IN] type #td_u32  specify the time, unit: us
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see serial_init|serial_putc|serial_puts|serial_put_buf|serial_getc|serial_tstc|serial_puthex 
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 serial_getc_timeout(td_u32 timeout_us);

/**************************************************memory management interface*****************************************/
/**
* @ingroup  hct_boot_api
* @brief pointer to the interface function of dynamic memory management module initialization
*
* @par description:
*      nothing
* @attention   nothing 
*
* @param  heap_start_addr [IN] type #td_u32  start memory address of heap management
* @param  heap_end_addr [IN] type #td_u32  end memory address of heap management
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
typedef td_void (*malloc_init_func)(td_u32 heap_start_addr, td_u32 heap_end_addr);
/**
* @ingroup  hct_boot_api
* @brief  pointer to the interface function of apply dynamic memory in boot
*
* @par description:
*         apply dynamic memory in boot
* @attention   nothing 
*
* @param  size [IN] type #td_u32 size of memory apply
*
* @retval td_void*  pointer to memory which applied success. If apply failed, return TD_NULL.
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
typedef td_void * (*malloc_func)(td_u32 size);
/**
* @ingroup  hct_boot_api
* @brief  pointer to interface function of release dynamic memory in boot.
*
* @par description:
*        release dynamic memory in boot.
* @attention   nothing 
*
* @param  addr [IN] type #td_void*  pointer to the memory address to be released. NOTION: do not release the same memory address multiple times.
*
* @retval #0     success
* @retval #-1    fialure
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
typedef td_s32 (*free_func)(td_void *addr);

/**
* @ingroup  hct_boot_api
*
* uapi_register_malloc interface input parameters which is used to customize the heap management policy.
 */

typedef struct
{
    malloc_init_func init;   /**< heap memory management initialization interface */
    malloc_func boot_malloc; /**< apply heap memory interface */
    free_func boot_free;     /**< release heap memory interface */
} uapi_malloc_func_s;

/**
* @ingroup  hct_boot_api
* @brief  register the functions related to malloc management. Register the functions fisrt and then use other interface.
*
* @par description:
*         Users can customize the memory management fucntions.
* @attention   nothing 
* @li pFuncs¡¢pFuncs->init,pFuncs->boot_malloc,pFuncs->boot_free can not be null, otherwise boot cannot run normally. 
*      Initialize configure function: The interface does not check the parameter.The validity of parameter ensured by invoker.
*
* @param  pFuncs [IN] type #uapi_malloc_func_s *  pointer to register structure of interface related ro apply memmory.
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see boot_malloc_init|boot_malloc|boot_free
* @since DW21_V100R001C00
 */

td_u32 uapi_register_malloc(uapi_malloc_func_s *pFuncs);

/**
* @ingroup  hct_boot_api
* @brief  interface of initializing dynamic memory management module in boot.Invoke uapi_register_malloc to register before use the interface.
*
* @par description:
*          Invoke this interface to initialize before using dynamic memory in boot.
* @attention   nothing 
*
* @param  heap_start_addr [IN] type #uintptr_t  start address of heap management
* @param  heap_end_addr [IN] type #uintptr_t  end address of heap management
*
* @retval nothing
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void boot_malloc_init(uintptr_t heap_start_addr, uintptr_t heap_end_addr);

/**
* @ingroup  hct_boot_api
* @brief  Apply dynamic memory in boot. Invoke uapi_register_malloc to register the interface before using it.
*
* @par description:
*        Apply dynamic memory in boot.
* @attention   nothing 
*
* @param  size [IN] type #td_u32 size of dynamic memory to be applied
*
* @retval td_void*  pointer to memory address which applied success. If applied failed, return TD_NULL.
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void *boot_malloc(td_u32 size);

/**
* @ingroup  hct_boot_api
* @brief  release dynamic memory in boot. Invoke uapi_register_malloc to register the interface before using it.
*
* @par description:
*       release dynamic memory in boot.
* @attention   nothing 
*
* @param  addr [IN] type #td_void*  pointer to memory address which to be released. Notion: do not release the same memory address multiple times.
*
* @retval #0      release memory success.
* @retval #-1     release memory failed.
*
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see uapi_register_malloc
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 boot_free(td_void *addr);

/**************************************************efuse interface **************************************************/
/**
* @ingroup  hct_boot_api
* 
* ID in efuse. EXT_EFUSE_XXX_RO_ID indicates that the area is read-only. EXT_EFUSE_XXX_RW_ID indicates that the area is readable and writable.
 */
typedef enum {
    EXT_EFUSE_CHIP_RO_ID = 0,              /**< DW21 chip ID */
    EXT_EFUSE_DIE_RO_ID = 1,               /**< DW21 the only identification of the chip */
    EXT_EFUSE_ROOT_PUBKEY_RW_ID = 2,       /**< root key sha256 */
    EXT_EFUSE_UART_PUBKEY_RW_ID = 3,       /**< value of RSA public key SHA256 used for UART connect authentication */
    EXT_EFUSE_BUCK_TRIM_RO_ID = 4,         /**< DC-DC voltage adjustment calibration value */
    EXT_EFUSE_TSENSOR_RO_ID = 5,           /**< temprature detection calibration value */
    EXT_EFUSE_OSC_TRIM_RO_ID = 6,          /**< 32.768K clock is used to provide the stable clock of some modules in deep sleep. This clock is used to calibrate the clock. */
    EXT_EFUSE_LDO_TRIM_RO_ID = 7,          /**< not support currently */
    EXT_EFUSE_RSIM_RW_ID = 8,              /**< not support currently */
    EXT_EFUSE_SUBKEY_CAT_RW_ID = 9,        /**< DW21 fixed to 0 */
    EXT_EFUSE_INSECURE_VERSION_RW_ID = 10, /**< it is used for differentiating management of R&D versions and production versions. */
    EXT_EFUSE_JTM_RW_ID = 11,              /**< JTAG Mask */
    EXT_EFUSE_UTM0_RW_ID = 12,             /**< UART0 Mask */
    EXT_EFUSE_UTM1_RW_ID = 13,             /**< UART0 Mask */
    EXT_EFUSE_SDC_RW_ID = 14,              /**< UART/JTAG authentication judgement switch */
    EXT_EFUSE_BOOT_SEL_RW_ID = 15,         /**< Boot start mode. 0:start from ROM Boot ,1£ºstart from Flash Boot directly */
    EXT_EFUSE_SECURE_BOOT_RW_ID = 16,      /**< secure boot switch */
    EXT_EFUSE_IDX_MAX,                     /**< max id in efuse. Can not be used */
} ext_efuse_idx_e;

/**
* @ingroup  hct_boot_api
* 
* @li efuse lock ID number. Unit of lock is 1bit corresponds to 128bits storage.
* @li enumerable ID number is combined on the usage of efuse storage area.
* @li EXT_EFUSE_LOCK_CHIP_DIE_X_ID indicates operate x bits.
 */
typedef enum {
    EXT_EFUSE_LOCK_CHIP_DIE_2_ID,                  /**< lock area EXT_EFUSE_CHIP_RO_ID  and  EXT_EFUSE_DIE_RO_ID */
    EXT_EFUSE_LOCK_ROOT_PUBKEY_2_ID,               /**< lock area EXT_EFUSE_ROOT_PUBKEY_RW_ID */
    EXT_EFUSE_LOCK_UART_PUBKEY_2_ID,               /**< lock area EXT_EFUSE_UART_PUBKEY_RW_ID */
    EXT_EFUSE_LOCK_BUCK_TSENSOR_OSC_LOD_TRIM_1_ID, /**< lock area between EXT_EFUSE_BUCK_TRIM_RO_ID and EXT_EFUSE_LDO_TRIM_RO_ID */
    EXT_EFUSE_LOCK_RSIM_SUBKEYCAT_VER_1_ID,        /**< lock area EXT_EFUSE_RSIM_RW_ID and EXT_EFUSE_INSECURE_VERSION_RW_ID */
    EXT_EFUSE_LOCK_JTM_UTM_1_ID,                   /**< lock area EXT_EFUSE_JTM_RW_ID and EXT_EFUSE_UTM1_RW_ID */
    EXT_EFUSE_LOCK_SDC_BOOTSEL_SECBOOT_1_ID,       /**< lock area EXT_EFUSE_SDC_RW_ID and EXT_EFUSE_SECURE_BOOT_RW_ID */
    EXT_EFUSE_LOCK_USER1_1_ID,                     /**< lock reserved area */
    EXT_EFUSE_LOCK_USER2_1_ID,                     /**< lock reserved area */
    EXT_EFUSE_LOCK_RESERVED1_1_ID,                 /**< lock reserved area */
    EXT_EFUSE_LOCK_RESERVED2_1_ID,                 /**< lock reserved area */
    EXT_EFUSE_LOCK_RESERVED3_1_ID,                 /**< lock reserved area */
    EXT_EFUSE_LOCK_MAX,                            /**< max lock ID value. Can not be used. */
} ext_efuse_lock_e;

/** 
* @ingroup  hct_boot_api
* area attribution,indicating whether the area can be read or written.
 */
#define EFUSE_IDX_NRW 0x0 /**<unreadable and writable */
#define EFUSE_IDX_RO  0x1 /**<read-only */
#define EFUSE_IDX_WO  0x2 /**<write-only */
#define EFUSE_IDX_RW  0x3 /**<readable and writable */

/**
* @ingroup  hct_boot_api
* @brief read efuse ID configuration.
*
* @par description:  
*         read efuse ID configuration.
* @attention nothing
* @param  idx[IN] type #ext_efuse_idx_e   efuse ID number
* @param  start_bit[OUT] type #td_u16 * indicates the ID start bit index of the area.
* @param  size[OUT] type #td_u16 * ID length of the area. Unit: bit.
* @param  attr[OUT] type #td_u8 * the area attribution. The value is #EFUSE_IDX_NRW|#EFUSE_IDX_RO|#EFUSE_IDX_WO|#EFUSE_IDX_RW.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_write|uapi_efuse_read¡£
* @since DW21_V100R001C00
 */

EXT_EXTERN td_s32 uapi_get_efuse_cfg_by_id(ext_efuse_idx_e idx, td_u16 *start_bit, td_u16 *size, td_u8 *attr);

/**
* @ingroup  hct_boot_api
* @brief read data from efuse.
*
* @par description:  
*           read data from efuse.
* @attention nothing
* @param  en_efuse_id[IN] type #ext_efuse_idx_e   efuse ID number.
* @param  data[OUT] type #td_u8 * Users invoke uapi_get_efuse_cfg_by_id first to get the length of this area, then apply a space and input the space address.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_write¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_read(ext_efuse_idx_e en_efuse_id, td_u8 *data);

/**
* @ingroup  hct_boot_api
* @brief write data to efuse.
*
* @par description: 
*            nothing
* @attention nothing
* @param  en_efuse_id[IN] type #ext_efuse_idx_e   efuse ID number.
* @param  data[IN] type #td_u8 * Users invoke uapi_get_efuse_cfg_by_id first to get the length of this area, then apply a space with data and input the space address.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_read¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_write(ext_efuse_idx_e en_efuse_id, td_u8 *data);

/**
* @ingroup  hct_boot_api
* @brief Lock an area in efuse. The area can not be written again after lock.
*
* @par description: 
*        Lock an area in efuse. The area can not be written again after lock. Each bit corresponds to 128bits storage.
* @attention The locking operation takes effect only after the system is restarted.
* @param  en_efuse_lock_id[IN] type #ext_efuse_idx_e  efuse ID number to be locked.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_write¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_lock(ext_efuse_lock_e en_efuse_lock_id);

/**
* @ingroup  hct_boot_api
* @brief Get efuse lock status. Query lock status of storage area.
*
* @par description: 
*          Get efuse lock status. Query lock status of storage area which is 16bits.
* @attention nothing
* @param  lock_stat[IN] type #td_u16   Get efuse lock status.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_write¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_get_lockstat(td_u16 *lock_stat);

/**
* @ingroup  hct_boot_api
* @brief Users read data from a specified area in efuse.
*
* @par description:  
*      Generally used to read data from user area in efuse. Also support read data from a specified address.
* @attention  The interface generally used to operate reserved area. It is recommended that use the uapi_efuse_read interface for other pre-allocated areas #ext_efuse_idx_e.
* @param  start_bit     [IN]type #td_u16 start bit. This address must be 8-bits-aligned.
* @param  size          [IN]type #td_u16 bit size to be read. The size is converted to 8-bits upward aligned by internal forcibly.
* @param  key_data     [OUT]type #td_u8 * Put the data which read success into the address. Users input the pointer to the space that applied before.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_usr_write¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_usr_read(td_u16 start_bit, td_u16 size, td_u8 *key_data);

/**
* @ingroup  hct_boot_api
* @brief Users write data to a specified area in efuse.
*
* @par description:
*         Gernerally used to write data to user area in efuse. Also support write data to a specified address.
* @attention 
*         @li Generally, the interface is used for operating the content in the user reserved area. It is recommended that use the uapi_efuse_write and uapi_efuse_lock for other pre-allocated areas #ext_efuse_idx_e.
*         @li support users write data to any address. The detailed address writting needs to check whether there is a conflict based on the solution document.
*         @li write operation is risky operation. For details, see <<DW21V100 API development guide>>.
* @param  start_bit[IN] type #td_u16   start bit. the value of start_bit add size must less than (2048-16). total size of efuse is 2048. The last 16bits is area for locking. Do not use the interface to operate.
* @param  size[IN] type #td_u16  length of bits to be written. support write a single bit. The max supportable length for writting is 256bits.
* @param  key_data[IN] type #td_u8 * put the data into this address which is to be written and max length is 32Bytes.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_efuse_usr_read¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_efuse_usr_write(td_u16 start_bit, td_u16 size, td_u8 *key_data);

/************************************************** random number interface **************************************************/

/**
* @ingroup  hct_boot_api
* @brief Obtain the real random number.
*
* @par description:
*       Obtain the real random number.
* @attention nothing
* @param  p_rng[OUT] type #td_u32*   The obtained real random number.
*
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  nothing
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_get_random(td_u32 *p_rng);

/**************************************************sha256  interface **************************************************/

/**
* @ingroup  hct_boot_api
* @brief calculate SHA256 value
*
* @par description:
*        calculate sha256 value of a specified data
* @attention nothing
* @param  p_input_data[IN] type #td_u8   input data to be calculated.
* @param  input_data_len[IN] type #td_u32  data length.
* @param  p_output_hash[OUT] type #td_u8 * SHA256 value that obtained by calculate.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_hash_sha256(td_u8 *p_input_data, td_u32 input_data_len, td_u8 *p_output_hash);

/**
* @ingroup  hct_boot_api
* @brief Start segment-based hash calculation.
*
* @par description:  
*       Start segment-based hash calculation.
* @attention nothing
* @param  nothing
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_hash_update|uapi_hash_final¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_hash_start(td_void);

/**
* @ingroup  hct_boot_api
* @brief Enter the source data to be calculated segment by segment.
*
* @par description:
*      Enter the source data to be calculated segment by segment.Invoke times by segment count.
* @attention nothing
* @param  p_input_data[IN] type #td_u8*   pointer to the input data.
* @param  input_data_len[IN] type #td_u32 input data length.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_hash_start|uapi_hash_final¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_hash_update(EXT_CONST td_u8 *p_input_data, td_u32 input_data_len);

/**
* @ingroup  hct_boot_api
* @brief After the segmentation calculation is completed, ouotput the hash result.
*
* @par description:
*      After the segmentation calculation is completed, ouotput the hash result.
* @attention nothing
* @param  p_output_hash[OUT] type #td_u8*  ouotput the hash result
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_hash_start|uapi_hash_update¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_hash_final(td_u8 *p_output_hash);

/**************************************************rsa  interface **************************************************/

/**
 * @ingroup hct_boot_api
 * RSA verify the signature Paddingtype
 */
typedef enum {
    EXT_MODE_PKCS_V15 = 0x00, /**< RSA_PKCS1 */
    EXT_MODE_PKCS_PSS = 0x01, /**< RSA_PSS */
} ext_rsa_padding_mode;

/**
* @ingroup  hct_boot_api
* @brief RSA verify the signature
*
* @par description:  
*         RSA verify the validity of signature
* @attention nothing
* @param  p_u8_n[IN] type #td_u8*   modulus.
* @param  p_u8_e[IN] type #td_u8*   public key.
* @param  key_len[IN] type #td_u32 modulus or public key length. Unit: byte, for example, if rsa2048 is used, enter 256.
* @param  padding_mode[IN] type #ext_rsa_padding_mode   padding type
* @param  p_u8_hash_data[IN] type #td_u8*   Hash value of the original text. This function compares the value.
* @param  p_u8_sign[IN] type #td_u8* the signature to be verified.

* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  
* @since DW21_V100R001C00
 */

EXT_EXTERN td_s32 uapi_rsa_verify(const td_u8 *const p_u8_n, const td_u8 *const p_u8_e, const td_u32 key_len,
                               ext_rsa_padding_mode padding_mode, td_u8 *p_u8_hash_data, const td_u8 *const p_u8_sign);

/**
* @ingroup  hct_boot_api
* @brief process interface of security authenticatioon when serial port enter command line.
*
* @par description: 
*        serial port authentication process. Invoke this function when serial port need authentication.
* @attention nothing
* @param  en_uart               [IN] type #ext_uart_e   the port number of the debug serail port.
* @param  interrupt_timeout_ms  [IN] type #td_u32      timeout time(ms) for waitting PC software burn send the interrupt frame.
* @retval #TD_TRUE      authentication passed
* @retval #TD_FALSE     authentication failed
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  
* @since DW21_V100R001C00
 */
EXT_EXTERN td_bool uart_auth_process(ext_uart_e en_uart, td_u32 interrupt_timeout_ms);

/* Security verification function of ROMBoot verify the flash. Non-external interfaces */
EXT_EXTERN td_s32 security_boot_verify(td_u8 *addr);

/**************************************************timecnt  interface **************************************************/

/**
* @ingroup  hct_boot_api
* @brief enable hardware timecnt to calculate.
*
* @par description:
*        enable hardware timecnt to calculate.
* @attention nothing
* @param  nothing
*
* @retval nothing
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_disable|uapi_time_counter_gettime|uapi_time_counter_getsec|uapi_time_counter_getms|uapi_time_counter_getus
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_time_count_enable(td_void);

/**
* @ingroup  hct_boot_api
* @brief disable timecnt
*
* @par description: 
*        disable timecnt¡£
* @attention nothing
* @param  nothing
*
* @retval nothing
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_enable|uapi_time_counter_gettime|uapi_time_counter_getsec|uapi_time_counter_getms|uapi_time_counter_getus
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_time_count_disable(td_void);

/**
* @ingroup  hct_boot_api
* @brief obtain the timecnt timing time.
*
* @par description:  
*        obtain the timecnt timing time.Includes 3 timing units: s, ms, us.
* @attention  us, ms and second are independent of each other. For example, when the time in ms is full 1000ms, it will not cleared. Instead, counted as second.
*
* @param  p_us[IN] type #td_u32*   us.
* @param  p_ms[IN] type #td_u32*   ms.
* @param  p_sec[IN] type #td_u32*  s.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_enable|uapi_time_count_disable|uapi_time_counter_getsec|uapi_time_counter_getms|uapi_time_counter_getus
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_time_counter_gettime(td_u32 *p_us, td_u32 *p_ms, td_u32 *p_sec);

/**
* @ingroup  hct_boot_api
* @brief obtain the timing time start from timecnt. Unit: s
*
* @par description:  
*        obtain the timing time start from timecnt. Unit: s
* @attention   nothing 
*
* @param  p_sec[IN] type #td_u32*   seconds
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_enable|uapi_time_count_disable|uapi_time_counter_gettime|uapi_time_counter_getms|uapi_time_counter_getus
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_time_counter_getsec(td_u32 *p_sec);

/**
* @ingroup  hct_boot_api
* @brief obtain the timing time start from timecnt. Unit: ms
*
* @par description:  
*          obtain the timing time start from timecnt. Unit: ms
* @attention   nothing 
*
* @param  p_ms[IN] type #td_u32*   milliseconds
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_enable|uapi_time_count_disable|uapi_time_counter_gettime|uapi_time_counter_getsec|uapi_time_counter_getus
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_time_counter_getms(td_u32 *p_ms);

/**
* @ingroup  hct_boot_api
* @brief obtain the timing time start from timecnt. Unit: us
*
* @par description:
*        obtain the timing time start from timecnt. Unit: us
* @attention   nothing 
*
* @param  p_us[IN] type #td_u32*   microseconds
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_time_count_enable|uapi_time_count_disable|uapi_time_counter_gettime|uapi_time_counter_getsec|uapi_time_counter_getms
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_time_counter_getus(td_u32 *p_us);

/************************************************* watchdog interface **************************************************/
/** 
* @ingroup hct_boot_api
* By default, watchdog configuration time is 22s.
* The time can not be modified after romboot startup. Users can invoke uapi_watchdog_enable to configure the time again in flashboot.
 */

#define WDG_TIME_US 22000000 /* By default, watchdog configuration time is 22s. */

/**
* @ingroup  hct_boot_api
* @brief enable watchdog
*
* @par description:
*        enable watchdog
* @attention   nothing 
*
* @param  u32over_time[IN] type #td_u32   watchdog timeout time. Watchdog reset when timeout.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_watchdog_feed|uapi_watchdog_disable
* @since DW21_V100R001C00
 */
EXT_EXTERN td_u32 uapi_watchdog_enable(td_u32 u32over_time);

/**
* @ingroup  hct_boot_api
* @brief    extend the time of watchdog not restart.
*
* @par description:
*       extend the time of watchdog not restart. The extened time is the latest configuraton of uapi_watchdog_enable.
* @attention   nothing 
*
* @param  nothing
* @retval nothing
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_watchdog_enable|uapi_watchdog_disable
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_feed(td_void);

/**
* @ingroup  hct_boot_api
* @brief disable watchdog
*
* @par description:
*      disable watchdog
* @attention   nothing 
*
* @param  nothing
* @retval nothing
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_watchdog_enable|uapi_watchdog_feed
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_watchdog_disable(td_void);

/*************************************************LED  interface **************************************************/

/**
 * @ingroup hct_boot_api
 * configure LED mode
 */
typedef enum {
    EXT_LED_MODE_HIGH = 0, /**< valid when pin output high */
    EXT_LED_MODE_LOW = 1,  /**< valid when pin output low */
    EXT_LED_MODE_MAX,      /**< Max value, can not be used */
} ext_led_mode_e;

/**
 * @ingroup hct_boot_api
 * LED number
 */
typedef enum {
    EXT_LED_IDX_0,   /**< LED1 */
    EXT_LED_IDX_1,   /**< LED2 */
    EXT_LED_IDX_2,   /**< LED3 */
    EXT_LED_IDX_3,   /**< LED4 */
    EXT_LED_IDX_4,   /**< LED5 */
    EXT_LED_IDX_MAX, /**< Max led number, can not be used */
} ext_led_idx_e;

/**
* @ingroup  hct_boot_api
* @brief configure LED mode
*
* @par description:
*         configure LED mode
* @attention   nothing 
*
* @param  en_mode_type[IN] type #ext_led_mode_e  pull-down control bit of the output pin.
* @retval nothing
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_led_flash|uapi_led_off|uapi_led_on
* @since DW21_V100R001C00
 */
EXT_EXTERN td_void uapi_led_config(ext_led_mode_e en_mode_type);

/**
* @ingroup  hct_boot_api
* @brief set the led to glitter
*
* @par description:
*      set the led to glitter
* @attention   nothing 
*
* @param  en_led_id[IN] type #ext_led_idx_e  LED number
* @param  on_ms[IN] type #td_u32  the lighted time of led when glitter.(ms)
* @param  off_ms[IN] type #td_u32  the extinguish time of led when glitter.(ms)
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_led_config|uapi_led_off|uapi_led_on
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_led_flash(ext_led_idx_e en_led_id, td_u32 on_ms, td_u32 off_ms);

/**
* @ingroup  hct_boot_api
* @brief set the led to off
*
* @par description:
*         set the led to off
* @attention   nothing 
*
* @param  en_led_id[IN] type #ext_led_idx_e  LED number
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_led_config|uapi_led_flash|uapi_led_on
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_led_off(ext_led_idx_e en_led_id);

/**
* @ingroup  hct_boot_api
* @brief set the led to on
*
* @par description:
*       set the led to on
* @attention   nothing 
*
* @param  en_led_id[IN] type #ext_led_idx_e  LED number
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  uapi_led_config|uapi_led_flash|uapi_led_off
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_led_on(ext_led_idx_e en_led_id);

/*************************************************CRC  interface **************************************************/

/* 16 bit CRC with polynomial x^16+x^12+x^5+1 (CRC-CCITT) */

/**
* @ingroup  hct_boot_api
* @brief crc16_ccitt calculation
*
* @par description:
*      crc16_ccitt calculation
* @attention   nothing 
*
* @param  crc_start[IN] type #td_u16  initial value
* @param  s[IN] type #td_uchar*  address of the original text which is to be calculated
* @param  len[IN] type #td_s32  length of the original text
* @retval  #value  crc16 value that calculated finished
* @par Dependency:
* <ul><li>soc_boot_rom.h: header file where the interface statement is located.</li></ul>
* @see  
* @since DW21_V100R001C00
 */

EXT_EXTERN td_u16 crc16_ccitt(td_u16 crc_start, td_uchar *s, td_s32 len);
/*************************************************GPIO  interface **************************************************/
/**
 * @ingroup hct_boot_api
 * GPIO number
 */
typedef enum {
    EXT_GPIO_IDX_0,   /**< GPIO0 */
    EXT_GPIO_IDX_1,   /**< GPIO1 */
    EXT_GPIO_IDX_2,   /**< GPIO2 */
    EXT_GPIO_IDX_3,   /**< GPIO3 */
    EXT_GPIO_IDX_4,   /**< GPIO4 */
    EXT_GPIO_IDX_5,   /**< GPIO5 */
    EXT_GPIO_IDX_6,   /**< GPIO6 */
    EXT_GPIO_IDX_7,   /**< GPIO7 */
    EXT_GPIO_IDX_8,   /**< GPIO8 */
    EXT_GPIO_IDX_9,   /**< GPIO9 */
    EXT_GPIO_IDX_10,  /**< GPIO10 */
    EXT_GPIO_IDX_11,  /**< GPIO11 */
    EXT_GPIO_IDX_12,  /**< GPIO12 */
    EXT_GPIO_IDX_13,  /**< GPIO13 */
    EXT_GPIO_IDX_14,  /**< GPIO14 */
    EXT_GPIO_IDX_15,  /**< GPIO15 */
    EXT_GPIO_IDX_16,  /**< GPIO16 */
    EXT_GPIO_IDX_17,  /**< GPIO17 */
    EXT_GPIO_IDX_18,  /**< GPIO18 */
    EXT_GPIO_IDX_19,  /**< GPIO19 */
    EXT_GPIO_IDX_20,  /**< GPIO20 */
    EXT_GPIO_IDX_21,  /**< GPIO21 */
    EXT_GPIO_IDX_22,  /**< GPIO22 */
    EXT_GPIO_IDX_MAX, /**< Max value, can not be used */
} ext_gpio_idx_e;

/**
 * @ingroup hct_boot_api
 * IO level status
 */
typedef enum {
    EXT_GPIO_VALUE0 = 0, /**< high level */
    EXT_GPIO_VALUE1,     /**< low level */
} ext_gpio_value_e;

/**
 * @ingroup hct_boot_api
 * IO direction
 */
typedef enum {
    EXT_GPIO_DIRECTION_IN = 0, /**< input direction */
    EXT_GPIO_DIRECTION_OUT,    /**< output direction */
} ext_gpio_direction_e;

/**
* @ingroup  hct_boot_api
* @brief  set the direction of the single IO pin.
*
* @par description:
*       set the direction of the single IO pin.
* @attention nothing
* @param  en_id [IN] type #ext_gpio_idx_e  IO index.
* @param  en_dir [IN] type #ext_gpio_direction_e  IO direction.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  EXT_MDM_IO_GetDir¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_set_dir(ext_gpio_idx_e en_id, ext_gpio_direction_e en_dir);
/**
* @ingroup  hct_boot_api
* @brief  obtain the direction of the single IO pin
*
* @par description:
*      obtain the direction of the single IO pin
* @attention nothing
* @param  en_id [IN] type #ext_gpio_idx_e   IO index
* @param  p_dir [OUT] type #ext_gpio_direction_e * IO direction
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_set_dir¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_get_dir(ext_gpio_idx_e en_id, ext_gpio_direction_e *p_dir);
/**
* @ingroup  hct_boot_api
* @brief  obtain the output level status of the single IO pin
*
* @par description:
*       obtain the output level status of the single IO pin
* @attention nothing
* @param  en_id [IN] type #ext_gpio_idx_e  IO index.
* @param  p_val [OUT] type #ext_gpio_value_e * output value. 1 indicates high level. 0 indicates low level.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_set_ouput_val |uapi_io_get_input_val¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_get_output_val(ext_gpio_idx_e en_id, ext_gpio_value_e *p_val);
/**
* @ingroup  hct_boot_api
* @brief  obtain the input level status of the single IO pin
*
* @par description:
*         obtain the input level status of the single IO pin
* @attention nothing
* @param  en_id [IN] type #ext_gpio_idx_e  IO index
* @param  p_val [OUT] type #ext_gpio_value_e * output value. 1 indicates high level. 0 indicates low level.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_set_ouput_val |uapi_io_get_output_val¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_get_input_val(ext_gpio_idx_e en_id, ext_gpio_value_e *p_val);
/**
* @ingroup  hct_boot_api
* @brief  set the output level status of the single IO pin
*
* @par description:
*         set the output level status of the single IO pin
* @attention nothing
* @param  en_id [IN] type #ext_gpio_idx_e  IO index
* @param  en_val [IN] type #ext_gpio_value_e   output value. 1 indicates high level. 0 indicates low level.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_get_input_val |uapi_io_get_output_val¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_set_ouput_val(ext_gpio_idx_e en_id, ext_gpio_value_e en_val);
/**
* @ingroup  hct_boot_api
* @brief obtain the IO pin multiplexing.
*
* @par description:
*         obtain IO function.
* @attention nothing
* @param  io_id [IN] type #ext_gpio_idx_e  io index
* @param  func_val [OUT] type #td_u8 *  io multiplexing
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_set_func¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_get_func(ext_gpio_idx_e io_id, td_u8 *func_val);
/**
* @ingroup  hct_boot_api
* @brief Set the IO pin multiplexing.
*
* @par description:
*          Set IO function.
* @attention nothing
* @param  io_id [IN] type #ext_gpio_idx_e  io index
* @param  func_val [IN] type #td_u8  io multiplexing
*                   @li For details about the multiplexing value, see section "Multiplexing register description" in the ¡¶Power Line Carrier Communication Chip User Guide¡·
*                   @li Users ensure the validity of the input register value. This interface does not check the validity of the settings.
* @retval #0      success.
* @retval #not 0     failed.
* @par Dependency:
* <ul><li>soc_boot_rom.h: This file is used for describing interfaces relates to IO.</li></ul>
* @see  uapi_io_get_func¡£
* @since DW21_V100R001C00
 */
EXT_EXTERN td_s32 uapi_io_set_func(ext_gpio_idx_e io_id, td_u8 func_val);

#endif

