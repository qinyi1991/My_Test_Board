/*
*********************************************************************************************************
*                                     SERIAL (BYTE) COMMUNICATION
*
*                         (c) Copyright 2007-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     SERIAL (BYTE) COMMUNICATION
*                                            DEVICE DRIVER
*
*                                                STM32
*                                          BSP FOR STM3240x
*
* Filename      : serial_bsp_stm3240x.h
* Version       : V2.00.01
* Programmer(s) : FGK
*                 FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_BSP_TEMPLATE present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_BSP_STM3240X_PRESENT
#define   SERIAL_BSP_STM3240X_PRESENT


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <bsp.h>
#include "serial_bsp_rs485.h"
#include  <serial_drv_stm32.h>


/*
*********************************************************************************************************
*                                           SERIAL DEVICES
*********************************************************************************************************
*/

void  BSP_Ser_Init (CPU_INT32U  baud_rate);
void  BSP_RS232_Init (CPU_INT32U  baud_rate);
void  BSP_RS232_WrStr (CPU_INT08U  *p_str);
void  BSP_RS232_Printf (CPU_INT08U  *format, ...);
void BSP_RS232_WrByte(CPU_INT08U  c);
/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/


#endif                                                          /* End of SERIAL_BSP_STM3240x module include.           */
