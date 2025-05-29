#ifndef DW21_H
#define DW21_H

#define __NVIC_PRIO_BITS          3U
#define __Vendor_SysTickConfig    0U

typedef enum IRQn
{
    /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
    NonMaskableInt_IRQn             = -14,    /*!< 2 Non Maskable Interrupt                             */
    MemoryManagement_IRQn           = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
    BusFault_IRQn                   = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
    UsageFault_IRQn                 = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
    SVCall_IRQn                     = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
    DebugMonitor_IRQn               = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
    PendSV_IRQn                     = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
    SysTick_IRQn                    = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */
    WatchDog_IRQn                   = 0,
    Phy_IRQn                        = 1,
    Chiper_IRQn                     = 2,
    Ssp0_IRQn                       = 3,
    I2c_IRQn                        = 4,
    Uart1_IRQn                      = 5,
    Uart0_IRQn                      = 6,
    Timer0_IRQn                     = 7,
    Timer1_IRQn                     = 8,
    Timer2_IRQn                     = 9,
    Timer3_IRQn                     = 10,
    Timer4_IRQn                     = 11,
    Timer5_IRQn                     = 12,
    Gpio_IRQn                       = 13,
    Sfc_IRQn                        = 14,
    Spacc_norm_IRQn                 = 15,
    Ssp1_IRQn                       = 16,
	IRQ_MAX_NUM                     = 17
} IRQn_Type;

#endif // DW21_H

