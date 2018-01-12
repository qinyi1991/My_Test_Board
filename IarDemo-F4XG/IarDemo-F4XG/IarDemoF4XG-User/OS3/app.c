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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <math.h>
#include  <stm32f4xx.h>

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>

#include  <app_cfg.h>
#include  <bsp.h>
#include  <serial_bsp_stm3240x.h>
#include "serial_bsp_rs485.h"
#include "bsp_DACoutput.h"
#include "bsp_EncoderScan.h"
#include "DataScope_DP.h"

Fixture_Status_Flag AppTask_Fixture_Status;
extern uint32_t DACCH1_DAT,DACCH2_DAT;

//外部引用声明
extern I2C_HandleTypeDef  I2cHandle; //实例化I2cHandle
extern uint8_t RFID_Buf[40];
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef RS232Handle;       
extern DAC_HandleTypeDef    DacHandle;

//小车断开连接
uint8_t Key_Switch_OFF_Delay;

//CPU_INT08U  SF_Version[16]={"MarsCard20170327"};
void PrintStringsToScreen(CPU_INT16S *Ptr_x,CPU_INT08U *ptr,CPU_INT08U size);
void SetOutput(void);


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_TASK_EQ_0_ITERATION_NBR              16u
#define  APP_TASK_EQ_1_ITERATION_NBR              18u


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* --------------- APPLICATION GLOBALS ---------------- */
static  OS_TCB       AppTaskStartTCB;
static  CPU_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

                                                                /* --------------- SEMAPHORE TASK TEST --------------- */

static  OS_TCB       AppTaskSerDataHandlingTCB;
static  CPU_STK      AppTaskSerDataHandlingStk[APP_CFG_TASK_SER_STK_SIZE];
///通信控制任务
OS_TCB   AppTaskSerDataHandlingTCB; 

static  OS_TCB       AppTaskRFIDCardHandlingTCB;
static  CPU_STK      AppTaskRFIDCardHandlingStk[APP_CFG_TASK_RFID_STK_SIZE];

static  OS_TCB       AppTaskPacientCartHandlingTCB;
static  CPU_STK      AppTaskPacientCartHandlingStk[APP_CFG_TASK_CART_STK_SIZE];

//定时扫描任务
#pragma data_alignment=8  //任务堆栈是8字节对齐，任务要使用浮点数输出
static  OS_TMR   AppSensorScanTCB;

#if (OS_CFG_SEM_EN > 0u)
static  OS_SEM       AppTraceSem;
#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart (void  *p_arg);
static  void  AppTaskCreate(void);
static  void  AppObjCreate (void);

static  void  AppTaskSerDataHandling     (void  *p_arg);
static  void  AppTaskRFIDCardHandling    (void  *p_arg);
static  void  AppTaskPacientCartHandling (void  *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 1) STM32F4xx HAL library initialization:
*                      a) Configures the Flash prefetch, intruction and data caches.
*                      b) Configures the Systick to generate an interrupt. However, the function ,
*                         HAL_InitTick(), that initializes the Systick has been overwritten since Micrium's
*                         RTOS has its own Systick initialization and it is recommended to initialize the
*                         Systick after multitasking has started.
*
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR   err;
    
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif
	
    HAL_Init();                                                 /* See Note 1.                                          */
    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    Math_Init();                                                /* Initialize Mathematical Module                       */
	
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameSet((CPU_CHAR *)"STM32F407IG",
                (CPU_ERR  *)&cpu_err);
#endif

    BSP_IntDisAll();                                            /* Disable all Interrupts.                              */
    
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    App_OS_SetAllHooks();

    OSTaskCreate(&AppTaskStartTCB,                              /* Create the start task                                */
                 "App Task Start",
                 AppTaskStart,
                 0u,
                 APP_CFG_TASK_START_PRIO,
                 &AppTaskStartStk[0u],
                 AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                 APP_CFG_TASK_START_STK_SIZE,
                 0u,
                 0u,
                 0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);
    
    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      err;
	
   (void)p_arg;
   

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    AppObjCreate();                                             /* Create Applicaiton kernel objects                    */
    AppTaskCreate();                                            /* Create Application tasks                             */

    BSP_Ser_Init(115200);
    BSP_RS232_Init(115200);
    BSP_RS485Port_Init(115200); 
    IIC_Configuration();
    HAL_DAC_ConfigInit(); //产生控制小车的模拟信号
    
    BSP_SPI2_Init();      //MES传感器信号
    
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(24u, 0u, 0u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    OS_ERR  os_err;

    //创建任务一：用于串行通信
    //与触控屏通信
    OSTaskCreate(&AppTaskSerDataHandlingTCB,
                 "SerDataHandling",
                 AppTaskSerDataHandling,
                 0,
                 APP_CFG_TASK_SER_PRIO,
                 &AppTaskSerDataHandlingStk[0],
                 AppTaskSerDataHandlingStk[APP_CFG_TASK_SER_STK_SIZE / 10u],
                 APP_CFG_TASK_SER_STK_SIZE,
                 0u,
                 0u,
                 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
    //创建任务二：用于RFID信息采集 
    //RS485
    OSTaskCreate(&AppTaskRFIDCardHandlingTCB,
                 "RFIDCardHandling",
                 AppTaskRFIDCardHandling,
                 0,
                 APP_CFG_TASK_RFID_PRIO,
                 &AppTaskRFIDCardHandlingStk[0],
                 AppTaskRFIDCardHandlingStk[APP_CFG_TASK_RFID_STK_SIZE / 10u],
                 APP_CFG_TASK_RFID_STK_SIZE,
                 0u,
                 0u,
                 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);

    //创建任务三：用于PacientCart控制
    OSTaskCreate(&AppTaskPacientCartHandlingTCB,
                 "PacientCartHandling",
                 AppTaskPacientCartHandling,
                 0,
                 APP_CFG_TASK_CART_PRIO,
                 &AppTaskPacientCartHandlingStk[0],
                 AppTaskPacientCartHandlingStk[APP_CFG_TASK_CART_STK_SIZE / 10u],
                 APP_CFG_TASK_CART_STK_SIZE,
                 0u,
                 0u,
                 0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
                 &os_err);
    
    //建立一个软件中断服务任务，定时10ms
    OSTmrCreate ((OS_TMR     *)          &AppSensorScanTCB,
                 (CPU_CHAR   *)          "App Sensor Scan",
                 (OS_TICK     )          1,
                 (OS_TICK     )          1, 
                 (OS_OPT      )          OS_OPT_TMR_PERIODIC,
                 (OS_TMR_CALLBACK_PTR)   Fixture_Sensor_Scan,
                 (void*      )           0,
                 (OS_ERR    *)           &os_err);
    
    OSTmrStart  ((OS_TMR     *)          &AppSensorScanTCB,
                 (OS_ERR     *)          &os_err);   
    
}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
    OS_ERR  os_err;
    
#if (OS_CFG_SEM_EN > 0u)
    OSSemCreate(&AppTraceSem,
                "Trace Lock",
                 1u,
                &os_err);
    
#endif

}

/*
*********************************************************************************************************
*                                          AppTaskSerDataHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskSerDataHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskSerDataHandling (void  *p_arg)
{
    OS_ERR  os_err;
    CPU_TS   ts;
    CPU_INT08U   *Ptr_CoilData;
    CPU_INT16S   *Ptr_RegData;
    
    (void)p_arg;
    Ptr_CoilData=CoilData;
    Ptr_RegData=RegData;
    HAL_I2C_Mem_Read_IT(&I2cHandle,0xA0,1,I2C_MEMADD_SIZE_16BIT,(uint8_t*)&RegData[1], 4); 
    PrintStringsToScreen(Ptr_RegData+NRegisAdd201,"hello word",16);
    
    
    while (DEF_TRUE) {
          OSTaskSemPend((OS_TICK )0,
                        (OS_OPT  )OS_OPT_PEND_BLOCKING,
                        (CPU_TS *)&ts,
                        (OS_ERR *)&os_err);
          
          if(os_err==OS_ERR_NONE){
               //处理数据
               checkComm0Modbus();   //modbus通信协议
               SetOutput();
               BSP_OUT01=~BSP_OUT01;
               receCount=0;
               
               
          }
          
          OSTimeDlyHMSM(0u, 0u, 0u, 10u,
                        OS_OPT_TIME_HMSM_STRICT,
                        &os_err);
    }
}


/*
*********************************************************************************************************
*                                          AppTaskRFIDCardHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskRFIDCardHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskRFIDCardHandling (void  *p_arg)
{
    OS_ERR       os_err;
    CPU_INT16S   *Ptr_RegData=RegData;
    CPU_INT32U   *ptr_data='\0';
    Encoder_left_Var=0; 
    while (DEF_TRUE) {
              PrintStringsToScreen(Ptr_RegData+NRegisAdd201,RFID_Buf,10);
              
              BSP_RS232_Printf("OK");
              //软件触发DAC转换
              DACCH2_DAT=*(Ptr_RegData+NRegisAdd102);
              DACCH1_DAT=*(Ptr_RegData+NRegisAdd101);
              
              if(Key_Switch_OFF_Delay>0){
                Key_Switch_OFF_Delay--;
                if(Key_Switch_OFF_Delay == 0){
                  BSP_OUT04=1;
                }
              }
              
              //串口示波器
             DataScopePrint((Encoder_left_Var)/1000.0,(Encoder_right_Var)/1000.0,Position_Left/1000.0,Position_Right/1000.0);      
             //数据写入串口屏
              ptr_data=(uint32_t*)(Ptr_RegData+NRegisAdd103);
              *ptr_data=Encoder_left_Var;
              *(Ptr_RegData+NRegisAdd105) =  (uint16_t)Encoder_right_Var;
              *(Ptr_RegData+NRegisAdd139) = 270;
              
              OSTimeDlyHMSM(0u, 0u, 0u,200u,
                            OS_OPT_TIME_HMSM_STRICT,
                            &os_err);
    }
}


/*
*********************************************************************************************************
*                                          AppTaskPacientCartHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskPacientCartHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/ 
void  AppTaskPacientCartHandling (void  *p_arg)
{
      OS_ERR      err;
      CPU_INT16S   *Ptr_RegData=RegData;
      DACCH1_DAT=0;
      DACCH2_DAT=0;
      
      AppTask_Fixture_Status.Start_Cart =0;
      
      Key_Switch_OFF_Delay=0;
      //小车控制系统
      while (DEF_TRUE) {
        
        BSP_OUT02=~BSP_OUT02;
        
        if(AppTask_Fixture_Status.Start_Cart == 1){
             AppTask_Fixture_Status.Start_Cart =0;
             DACCH1_DAT=0x0;
             DACCH2_DAT=0x0;
             BSP_OUT04=0;
             AppTask_Fixture_Status.AutoMode =1;
        }
        
        if(AppTask_Fixture_Status.AutoMode == 1 && AppTask_Fixture_Status.Stop_Cart == 0){
            //DACCH1 通道
            if(DACCH1_DAT<*(Ptr_RegData+SRegisAdd1)){DACCH1_DAT+=10;}
            else { DACCH1_DAT=*(Ptr_RegData+SRegisAdd1);}
            HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1,DAC_ALIGN_12B_R,DACCH1_DAT);
            *(Ptr_RegData+NRegisAdd101)=DACCH1_DAT;
            
            //DACCH2 通道
            if(DACCH2_DAT<*(Ptr_RegData+SRegisAdd2)){DACCH2_DAT+=10;}
            else{ DACCH2_DAT = *(Ptr_RegData+SRegisAdd2);}
            HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2,DAC_ALIGN_12B_R,DACCH2_DAT);
            *(Ptr_RegData+NRegisAdd102)=DACCH2_DAT;
            
            (DacHandle.Instance)->SWTRIGR = 0x03;
            
        }else if(AppTask_Fixture_Status.Stop_Cart == 1){
             DACCH1_DAT=0x0;
             DACCH2_DAT=0x0;
             HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1,DAC_ALIGN_12B_R,DACCH1_DAT);
             HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2,DAC_ALIGN_12B_R,DACCH2_DAT);
             (DacHandle.Instance)->SWTRIGR = 0x03;
             *(Ptr_RegData+NRegisAdd101)=DACCH1_DAT;
             *(Ptr_RegData+NRegisAdd102)=DACCH2_DAT;
             AppTask_Fixture_Status.AutoMode  =0;
             AppTask_Fixture_Status.Stop_Cart =0;
             Key_Switch_OFF_Delay=20;
              
        }
        if((DACCH1_DAT == *(Ptr_RegData+SRegisAdd1)) && (DACCH2_DAT == *(Ptr_RegData+SRegisAdd2))){AppTask_Fixture_Status.AutoMode=0;}
       
        //更新到输出口
        //CH1输出电压=（3.311*DACCH1_DAT）/4095;
       
       
        //读取编码器
        OSTimeDlyHMSM(0u, 0u, 0u, 50u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
      }
}

//显示缓冲区起点位置指针,显示内容起点指针，显示位宽
void PrintStringsToScreen(CPU_INT16S *Ptr_x,CPU_INT08U *ptr,CPU_INT08U size)
{
        CPU_INT08U   Chcounter; 
        //打印版本信息 NRegisAdd121
        for(Chcounter=0;Chcounter<size ;Chcounter+=2){
          //HMI设置中 高字节显示在右边，低字节显示在左边
          //下面运算的翻转高低字节，使得数据按照正常语序排列显示
          *(Ptr_x+Chcounter/2)=((CPU_INT16U)(*(ptr+1)<<8)|((CPU_INT16U)(*ptr)));
           ptr=ptr+2;                   
        }  
}

//单步调试控件
//输出控制
void SetOutput(void)
{
   CPU_INT08U   *Ptr_CoilData;
   Ptr_CoilData=CoilData;               //触控 按键事件 处理
   CPU_INT16S   *Ptr_RegData=RegData;
   if(Coil.Add>0)          
   {      
     //按钮状态扫描------------------------------------------------------------
    switch(Coil.Add)    
    {
        case NCoilAdd401:
        {
              if(*(Ptr_CoilData+NCoilAdd401) == 1)
              {
                 AppTask_Fixture_Status.Clear_RFIDBuf=1;
              }
//              else
//              {
//                 AppTask_Fixture_Status.Clear_RFIDBuf=0;
//                 BSP_OUT16=1;
//                 Clear_RFIDBuf();
//              }
        }break; 
        
        //强制端口输出
        case NCoilAdd402:
        {
              if(*(Ptr_CoilData+NCoilAdd402) == 1)
              {
                    if(*(Ptr_RegData+SRegisAdd1)<4095){ 
                         *(Ptr_RegData+SRegisAdd1)+=1;
                    }
                    else{ *(Ptr_RegData+SRegisAdd1)=4095;}
                    HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1,DAC_ALIGN_12B_R,*(Ptr_RegData+SRegisAdd1));
              }
//              else
//              {
//                
//              }
        }break;   
        case NCoilAdd403:
        {
              if(*(Ptr_CoilData+NCoilAdd403) == 1)
              {
                if(*(Ptr_RegData+SRegisAdd1)>100){
                        *(Ptr_RegData+SRegisAdd1)-=1;
                }
                else{ *(Ptr_RegData+SRegisAdd1)=100;}
                HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_1,DAC_ALIGN_12B_R,*(Ptr_RegData+SRegisAdd1));
              }
              
//              else
//              {
//                
//                
//              }
        }break; 
         case NCoilAdd404:
        {
              if(*(Ptr_CoilData+NCoilAdd404) == 1)
              {
                  if(*(Ptr_RegData+SRegisAdd2)<4095){ *(Ptr_RegData+SRegisAdd2)+=1;}
                  else{ *(Ptr_RegData+SRegisAdd2)=4095;}
                  HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2,DAC_ALIGN_12B_R,*(Ptr_RegData+SRegisAdd2));
              }
              
//              else
//              {
//                
//              }
        }break; 
        case NCoilAdd405:
        {
              if(*(Ptr_CoilData+NCoilAdd405) == 1)
              {
                if(*(Ptr_RegData+SRegisAdd2)>100){ *(Ptr_RegData+SRegisAdd2)-=1;}
                else{ *(Ptr_RegData+SRegisAdd2)=100;}
                HAL_DAC_SetValue(&DacHandle, DAC_CHANNEL_2,DAC_ALIGN_12B_R,*(Ptr_RegData+SRegisAdd2));
              }
              
//              else
//              {
//                
//                
//              }
        }break;
        case NCoilAdd406:
        {
              if(*(Ptr_CoilData+NCoilAdd406) == 1)
              {
                    AppTask_Fixture_Status.Start_Cart = 1;
                
              }
//              else
//              {
//                 
//                
//              }
        }break;
           case NCoilAdd407:
        {
              if(*(Ptr_CoilData+NCoilAdd407) == 1)
              {
                 AppTask_Fixture_Status.Stop_Cart =1;
              }
//              else
//              {
//                 BSP_OUT06=1; 
//                
//              }
        }break;
           case NCoilAdd508:
        {
          if(*(Ptr_CoilData+NCoilAdd508) == 1)
          {
    
          }
          else
          {
            
          }
        }break;
        case NCoilAdd509:
        {
          if(*(Ptr_CoilData+NCoilAdd509) == 1)
          {
           
          }
          else
          {
            
          }
        }break;
           //DACCH1_DAT=*(Ptr_RegData+SRegisAdd1);
	  // DACCH2_DAT=*(Ptr_RegData+SRegisAdd2);	
     }  
      Coil.Add=0;
      Coil.num=0;
  
   }
}



