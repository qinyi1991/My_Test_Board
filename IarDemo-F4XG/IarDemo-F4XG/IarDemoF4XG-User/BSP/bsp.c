/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
#include  <bsp_os.h>
#include  <stm32f4xx.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_BIT_RCC_PLLCFGR_PLLM                8u
#define  BSP_BIT_RCC_PLLCFGR_PLLN              336u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                2u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                7u

//输入PIN脚
//The fist input group
#define  BSP_GPIOB_INPUT01                      DEF_BIT_01
#define  BSP_GPIOF_INPUT02                      DEF_BIT_12
#define  BSP_GPIOF_INPUT03                      DEF_BIT_13
#define  BSP_GPIOF_INPUT04                      DEF_BIT_14
#define  BSP_GPIOF_INPUT05                      DEF_BIT_15
#define  BSP_GPIOG_INPUT06                      DEF_BIT_00
#define  BSP_GPIOG_INPUT07                      DEF_BIT_01
#define  BSP_GPIOD_INPUT08                      DEF_BIT_11
//The second input group
#define  BSP_GPIOD_INPUT09                      DEF_BIT_12
#define  BSP_GPIOD_INPUT10                      DEF_BIT_13
#define  BSP_GPIOG_INPUT11                      DEF_BIT_02
#define  BSP_GPIOG_INPUT12                      DEF_BIT_03
#define  BSP_GPIOG_INPUT13                      DEF_BIT_04
#define  BSP_GPIOG_INPUT14                      DEF_BIT_05
#define  BSP_GPIOG_INPUT15                      DEF_BIT_06
#define  BSP_GPIOG_INPUT16                      DEF_BIT_07

//输出PIN脚
//The first out group
#define  BSP_GPIOD_OUT01                        DEF_BIT_07
#define  BSP_GPIOG_OUT02                        DEF_BIT_09
#define  BSP_GPIOG_OUT03                        DEF_BIT_10
#define  BSP_GPIOG_OUT04                        DEF_BIT_11
//#define  BSP_GPIOE_OUT05                        DEF_BIT_08
//#define  BSP_GPIOE_OUT06                        DEF_BIT_11
//#define  BSP_GPIOE_OUT07                        DEF_BIT_12
//#define  BSP_GPIOE_OUT08                        DEF_BIT_13

#define  BSP_GPIOD_CH1_CLK                        DEF_BIT_05
#define  BSP_GPIOD_CH2_CLK                        DEF_BIT_06

#define  BSP_GPIOB_CH4_DAT                        DEF_BIT_07
#define  BSP_GPIOB_CH3_DAT                        DEF_BIT_06
//RS485 USART6 PC6-TX,PC7-RX
#define  BSP_RS485_EN                           DEF_BIT_08     


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)

/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_GPIO_Init        (void);


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*               (3) The voltage scaling allows optimizing the power consumption when the device is
*                   clocked below the maximum system frequency, to update the voltage scaling value
*                   regarding system frequency refer to product datasheet.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    RCC_OscInitTypeDef  RCC_OscInitStruct;
    RCC_ClkInitTypeDef  RCC_ClkInitStruct;

    BSP_IntInit();

    HAL_RCC_DeInit();

    __HAL_RCC_PWR_CLK_ENABLE();                                 /* Enable Power Control clock.                          */
                                                                /* See Note 3.                                          */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

                                                                /* PLLCLK    = HSE * (PLLN / PLLM)      = 336MHz.       */
                                                                /* SYSCLK    = PLLCLK / PLLP            = 168MHz.       */
                                                                /* OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.       */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = BSP_BIT_RCC_PLLCFGR_PLLM;
    RCC_OscInitStruct.PLL.PLLN       = BSP_BIT_RCC_PLLCFGR_PLLN;
    RCC_OscInitStruct.PLL.PLLP       = BSP_BIT_RCC_PLLCFGR_PLLP;
    RCC_OscInitStruct.PLL.PLLQ       = BSP_BIT_RCC_PLLCFGR_PLLQ;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);


    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_HCLK   |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;          /* HCLK    = AHBCLK  = PLL / AHBPRES(1) = 168MHz.       */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           /* APB1CLK = AHBCLK  / APB1DIV(4)       = 42MHz (max).  */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           /* APB2CLK = AHBCLK  / APB2DIV(2)       = 84MHz.        */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

                                                                /* STM32F405x/407x/415x/417x Revision Z devices: ...... */
    if (HAL_GetREVID() == 0x1001)                               /* ....prefetch is supported                            */
    {
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();                     /* Enable the Flash prefetch                            */
    }

    BSP_GPIO_Init();                                             /* Init LEDs.                                           */

		
#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    /* Enable tracing (see Note #2).                        */
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    /* Clr trace mode sel bits.                             */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; /* Cfg trace mode to synch 4-bit.                       */
#endif
		
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  hclk_freq;

    hclk_freq = HAL_RCC_GetHCLKFreq();
    return (hclk_freq);
}


/*
*********************************************************************************************************
*                                            HAL_InitTick()
*
* Description : This function has been overwritten from the STM32F4xx HAL libraries because Micrium's RTOS
*               has its own Systick initialization and because it is recomended to initialize the tick after
*               multi-tasking has started.
*
* Argument(s) : TickPriority          Tick interrupt priority.
*
* Return(s)   : HAL_OK.
*
* Caller(s)   : HAL_InitTick ()) is called automatically at the beginning of the program after reset by
*               HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
*
* Note(s)     : none.
*********************************************************************************************************
*/

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    if (OSRunning > 0u) {                                       /*Check if multi-tasking has started.                   */
        BSP_Tick_Init();
    }

    return (HAL_OK);
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}


/*
*********************************************************************************************************
*                                           BSP_GPIO_Init()
*
* Description : Initialize any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    inialize ALL  OUTs
*                       1    inialize user OUT1
*                       2    inialize user OUT2
*                       3    inialize user OUT3
*                       4    inialize user OUT4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void  BSP_GPIO_Init()
{
    GPIO_InitTypeDef  gpio_init;

    //输入
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOG);                    
    gpio_init.Pin   = BSP_GPIOG_INPUT11 | BSP_GPIOG_INPUT12| BSP_GPIOG_INPUT13| BSP_GPIOG_INPUT14 |BSP_GPIOG_INPUT15| BSP_GPIOG_INPUT16|BSP_GPIOB_CH4_DAT|BSP_GPIOB_CH3_DAT;
    gpio_init.Mode  = GPIO_MODE_INPUT;
    gpio_init.Pull  = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOG, &gpio_init);
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOB);
    
    gpio_init.Pin = BSP_GPIOB_INPUT01;
    HAL_GPIO_Init(GPIOB, &gpio_init);
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOD);
    
    gpio_init.Pin =BSP_GPIOD_INPUT08| BSP_GPIOD_INPUT09| BSP_GPIOD_INPUT10;
    HAL_GPIO_Init(GPIOD, &gpio_init);
    
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOG);
    gpio_init.Pin = BSP_GPIOG_INPUT06 |BSP_GPIOG_INPUT07;
    HAL_GPIO_Init(GPIOG, &gpio_init);	
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOF);
    gpio_init.Pin =BSP_GPIOF_INPUT02| BSP_GPIOF_INPUT03 |BSP_GPIOF_INPUT04 |BSP_GPIOF_INPUT05;
    HAL_GPIO_Init(GPIOF, &gpio_init);		
    
    //输出
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOD);                    
    gpio_init.Pin   = BSP_GPIOD_OUT01|BSP_GPIOD_CH1_CLK|BSP_GPIOD_CH2_CLK;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &gpio_init);
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOG);                    
    gpio_init.Pin   = BSP_GPIOG_OUT02 |BSP_GPIOG_OUT03|BSP_GPIOG_OUT04;
    HAL_GPIO_Init(GPIOG, &gpio_init);
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOC);                    
    gpio_init.Pin   = BSP_RS485_EN;
    HAL_GPIO_Init(GPIOC, &gpio_init);    
    
    BSP_OUT01=1;BSP_OUT02=1;BSP_OUT03=1;BSP_OUT04=1;
    BSP_CLK1 =1;BSP_CLK2=1;		
}
