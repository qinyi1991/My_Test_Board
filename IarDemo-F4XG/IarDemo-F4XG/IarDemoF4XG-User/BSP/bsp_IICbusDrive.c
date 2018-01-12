
#include <bsp_iicbusDrive.h>
#include <stm32f4xx.h>
#include "os.h"

I2C_HandleTypeDef  I2cHandle; //实例化I2cHandle

//I2C中断处理函数
void BSP_I2C3_ISR_Handler(void);
void BSP_I2C3_Error_Handler(void);

void IIC_Configuration(void)
{
    /*##-1- Configure the I2C peripheral ######################################*/
    I2cHandle.Instance             = I2Cx;
    
    I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    I2cHandle.Init.ClockSpeed      = 400000;
    I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    I2cHandle.Init.OwnAddress1     = I2C_ADDRESS;
    I2cHandle.Init.OwnAddress2     = 0xfe;
    
    if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
    {
      /* Initialization Error */
        BSP_I2C3_Error_Handler();    
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{  
      GPIO_InitTypeDef  GPIO_InitStruct;
      
      /*##-1- Enable GPIO Clocks #################################################*/
      /* Enable GPIO TX/RX clock */
      I2Cx_SCL_GPIO_CLK_ENABLE();
      I2Cx_SDA_GPIO_CLK_ENABLE();
      
      /*##-2- Configure peripheral GPIO ##########################################*/  
      /* I2C TX GPIO pin configuration  */
      GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
      GPIO_InitStruct.Alternate = I2Cx_SCL_AF;
      HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);
        
      /* I2C RX GPIO pin configuration  */
      GPIO_InitStruct.Pin = I2Cx_SDA_PIN;
      GPIO_InitStruct.Alternate = I2Cx_SDA_AF;
      HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

      /*##-3- Enable I2C peripherals Clock #######################################*/
      /* Enable I2C1 clock */
      I2Cx_CLK_ENABLE();
          
      /*##-4- Configure the NVIC for I2C #########################################*/   
      /* NVIC for I2C1 */
      HAL_NVIC_SetPriority(I2Cx_ER_IRQn, 0, 2);
      BSP_IntVectSet(BSP_INT_ID_I2C3_EV, BSP_I2C3_ISR_Handler);
      BSP_IntEn(BSP_INT_ID_I2C3_EV); //使能中断
      
      HAL_NVIC_SetPriority(I2Cx_EV_IRQn, 0, 3);
      BSP_IntVectSet(BSP_INT_ID_I2C3_ER, BSP_I2C3_ISR_Handler);
      BSP_IntEn(BSP_INT_ID_I2C3_ER); //使能中断
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);
  
  /*##-3- Disable the NVIC for I2C ###########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_ER_IRQn);
  HAL_NVIC_DisableIRQ(I2Cx_EV_IRQn);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
 
  
}

void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  
  
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  
  
}

void BSP_I2C3_ISR_Handler(void)
{
   HAL_I2C_ER_IRQHandler(& I2cHandle);
   HAL_I2C_EV_IRQHandler(& I2cHandle);
}

void BSP_I2C3_Error_Handler(void)
{

}


