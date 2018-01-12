#include "serial_bsp_stm3240x.h"
#include  <os.h>

uint8_t Rs232_Usart[50];

/* UART handler declaration */
UART_HandleTypeDef UartHandle;
UART_HandleTypeDef RS232Handle;
extern OS_TCB   AppTaskSerDataHandlingTCB; 


void BSP_Ser_ISR_Handler();
void BSP_RS485_ISR_Handler();
void BSP_RS232_ISR_Handler();

void  BSP_Ser_Init (CPU_INT32U  baud_rate)
{
                                               /* ----------------- INIT USART STRUCT ---------------- */
    UartHandle.Instance          = UART4;
    
    UartHandle.Init.BaudRate     = baud_rate;
    UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits     = UART_STOPBITS_1;
    UartHandle.Init.Parity       = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode         = UART_MODE_TX_RX;
    UartHandle.Init.OverSampling = UART_OVERSAMPLING_8;
        
                                              /* ------------------ SETUP USART2 -------------------- */
    if(HAL_UART_Init(&UartHandle) != HAL_OK){
      
    }

    if(HAL_UART_Receive_IT(&UartHandle, (uint8_t*)receBuf, 8)!= HAL_OK){
    
    }//使能接收中断  
}

void  BSP_RS232_Init (CPU_INT32U  baud_rate)
{
                                               /* ----------------- INIT USART STRUCT ---------------- */
    RS232Handle.Instance          = USART2;
    
    RS232Handle.Init.BaudRate     = baud_rate;
    RS232Handle.Init.WordLength   = UART_WORDLENGTH_8B;
    RS232Handle.Init.StopBits     = UART_STOPBITS_1;
    RS232Handle.Init.Parity       = UART_PARITY_NONE;
    RS232Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    RS232Handle.Init.Mode         = UART_MODE_TX_RX;
    RS232Handle.Init.OverSampling = UART_OVERSAMPLING_8;
        
                                              /* ------------------ SETUP USART2 -------------------- */
    if(HAL_UART_Init(&RS232Handle) != HAL_OK){
      
    }

   if(HAL_UART_Receive_IT(&RS232Handle, (uint8_t*)Rs232_Usart, 8)!= HAL_OK){
   
   }//使能接收中断  
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
        GPIO_InitTypeDef  GPIO_InitStruct;
        
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        BSP_PeriphEn(BSP_PERIPH_ID_GPIOA);                          /* Enable GPIOA clock.                                  */
        BSP_PeriphEn(BSP_PERIPH_ID_GPIOC);                          /* Enable GPIOA clock.                                  */
        /* Enable UART4 clock */
        BSP_PeriphEn(BSP_PERIPH_ID_USART4);                         /* Enable UART clock.                                    */
        BSP_PeriphEn(BSP_PERIPH_ID_USART2);                         /* Enable USART clock.                                    */
        BSP_PeriphEn(BSP_PERIPH_ID_USART6);                         /* Enable USART clock.                                    */
        /*##-2- Configure peripheral GPIO ##########################################*/  
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_0;   //PA0
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin       = GPIO_PIN_2;   //PA2
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin       = GPIO_PIN_6;   //PC6
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
        
        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin = GPIO_PIN_1;  //PA1
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        
        GPIO_InitStruct.Pin = GPIO_PIN_3;  //PA3
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 
        
        GPIO_InitStruct.Pin = GPIO_PIN_7;  //PC7
        GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 
        
        /*##-3- Configure the NVIC for UART ########################################*/   
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(UART4_IRQn, 0, 1);
        //HAL_NVIC_EnableIRQ(UART4_IRQn);    
        BSP_IntVectSet(BSP_INT_ID_USART4, BSP_Ser_ISR_Handler);
        BSP_IntEn(BSP_INT_ID_USART4); //使能中断
        
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 2);
        //HAL_NVIC_EnableIRQ(USART7_IRQn);    
        BSP_IntVectSet(BSP_INT_ID_USART2, BSP_RS232_ISR_Handler);
        BSP_IntEn(BSP_INT_ID_USART2); //使能中断

        HAL_NVIC_SetPriority(USART6_IRQn, 0, 3);
        //HAL_NVIC_EnableIRQ(USART7_IRQn);    
        BSP_IntVectSet(BSP_INT_ID_USART6, BSP_RS485_ISR_Handler);
        BSP_IntEn(BSP_INT_ID_USART6); //使能中断
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_UART4_FORCE_RESET();
    __HAL_RCC_UART4_RELEASE_RESET();
    
    __HAL_RCC_USART2_FORCE_RESET();
    __HAL_RCC_USART2_RELEASE_RESET();
 
    __HAL_RCC_USART6_FORCE_RESET();
    __HAL_RCC_USART6_RELEASE_RESET();
    
    /*##-2- Disable peripherals and GPIO Clocks #################################*/
    /* Configure UART Tx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);    
    /* Configure UART Rx as alternate function  */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7);    
    /*##-3- Disable the NVIC for UART ##########################################*/
    HAL_NVIC_DisableIRQ(UART4_IRQn);
    HAL_NVIC_DisableIRQ(USART2_IRQn);
    HAL_NVIC_DisableIRQ(USART6_IRQn);
}
//
void BSP_RS232_WrByte(CPU_INT08U  c)
{
      (RS232Handle.Instance)->DR=c;
      while (__HAL_UART_GET_FLAG(&RS232Handle, UART_FLAG_TC) == RESET);
      __HAL_UART_CLEAR_FLAG(&RS232Handle,UART_FLAG_TC); 	
}

void  BSP_RS232_WrStr (CPU_INT08U  *p_str)
{
  
    if (p_str == (CPU_INT08U *)0) {
        return;
    }
       
    while ((*p_str) != (CPU_INT08U )0) {
            BSP_RS232_WrByte(*p_str++);
    }
    
}

void  BSP_RS232_Printf (CPU_INT08U  *format, ...)
{
    CPU_INT08U  buf_str[100 + 1u];
    va_list   v_args;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

    BSP_RS232_WrStr(buf_str);
}
/*
*********************************************************************************************************
*                                         BSP_Ser_ISR_Handler()
*
* Description : Serial ISR.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
************************* ********************************************************************************
*/  
void  BSP_Ser_ISR_Handler (void)
{
   // HAL_UART_IRQHandler(&UartHandle); 
    uint32_t tmp1 = 0, tmp2 = 0;
    uint16_t *tmp_rx;
    UART_HandleTypeDef *huart=&UartHandle;
    OS_ERR err;
 
    //溢出错误
    tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
    tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
    /* UART Over-Run interrupt occurred ----------------------------------------*/
    if((tmp1 != RESET) && (tmp2 != RESET))
    { 
      __HAL_UART_CLEAR_OREFLAG(huart);
      huart->ErrorCode |= HAL_UART_ERROR_ORE;
    }

    //使用时隙断句法，判断接受到一帧数据
    tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
    tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
    /* UART in mode Receiver ---------------------------------------------------*/
    if((tmp1 != RESET) && (tmp2 != RESET))
    { 
          receTimeOut = 14;  //时隙设置 
          tmp_rx = (uint16_t*) huart->pRxBuffPtr;
          *tmp_rx = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
          huart->pRxBuffPtr += 1;
          if( ++receCount >7 ){
            
            OSTaskSemPost((OS_TCB *)&AppTaskSerDataHandlingTCB,
                          (OS_OPT  )OS_OPT_POST_NONE,
                          (OS_ERR *)&err); 
            
          }
    }
    
    //发送数据
    tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
    tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);
    /* UART in mode Transmitter ------------------------------------------------*/
    if((tmp1 != RESET) && (tmp2 != RESET))
    {
          huart->Instance->DR = (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0x00FF);
          if(--huart->TxXferCount == 0)
          {
            /* Disable the UART Transmit Complete Interrupt */
            __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
            /* Enable the UART Transmit Complete Interrupt */    
            __HAL_UART_ENABLE_IT(huart, UART_IT_TC);
          }
    }
    
    tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
    tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);
    /* UART in mode Transmitter end --------------------------------------------*/
    if((tmp1 != RESET) && (tmp2 != RESET))
    {
       __HAL_UART_DISABLE_IT(huart, UART_IT_TC);
       huart->State = HAL_UART_STATE_READY;
    }    
    
  
}

void BSP_RS485_ISR_Handler(void)
{
   HAL_UART_IRQHandler(&RS485Handle);
}

//rs232
void BSP_RS232_ISR_Handler(void)
{ 
      // HAL_UART_IRQHandler(&UartHandle); 
//      uint32_t tmp1 = 0, tmp2 = 0;
//      uint16_t *tmp_rx;
//      UART_HandleTypeDef *huart=&RS232Handle;
//      OS_ERR err;
//
//      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
//      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
//      /* UART in mode Receiver ---------------------------------------------------*/
//      if((tmp1 != RESET) && (tmp2 != RESET))
//      { 
//        tmp_rx = (uint16_t*) huart->pRxBuffPtr;
//        *tmp_rx = (uint16_t)(huart->Instance->DR & (uint16_t)0x00FF);
////        huart->pRxBuffPtr += 1;
////        if( ++receCount >7 ){
////          
////        }
//          __HAL_UART_CLEAR_FLAG(&RS232Handle,UART_FLAG_RXNE); 
//        
//      }
     HAL_UART_IRQHandler(&RS232Handle);
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
    // if(UartHandle == &RS232Handle) {}
  
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    OS_ERR err;
    
    if(huart == &RS485Handle){
      if(HAL_UART_Receive_IT(huart, (uint8_t*)RFID_Buf, RFID_BUF_SIZE)!= HAL_OK){
   
      }//使能接收中断  
       RS485Processing();
    }
    if(huart == &RS232Handle){
      if(HAL_UART_Receive_IT(&RS232Handle, (uint8_t*)Rs232_Usart, 8)!= HAL_OK){
        
      }//使能接收中断  
      
    }
    
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
 void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  
    if(huart == &UartHandle){ //uart4
      if(HAL_UART_Receive_IT(huart, (uint8_t*)receBuf, 8)!= HAL_OK){ }//使能接收中断  
      if(huart->State == UART_FLAG_ORE){
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
      }
       
    }
      
}
