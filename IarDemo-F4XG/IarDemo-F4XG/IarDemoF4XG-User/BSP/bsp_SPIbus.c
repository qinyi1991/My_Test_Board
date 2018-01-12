#include "bsp_SPIbus.h"

#define SPIx_TIMEOUT_MAX          0x1000    

//spi2初始化
SPI_HandleTypeDef hspi2;
static DMA_HandleTypeDef hdma_spi1_tx;
static DMA_HandleTypeDef hdma_spi1_rx;
uint32_t SpixTimeout = SPIx_TIMEOUT_MAX;
//uint8_t aRxBuffer[BUFFERSIZE];
void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream4_IRQHandler(void);
static void SPI_Error_Handler(void);

/* SPI2 init function */
void BSP_SPI2_Init(void)
{
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi2.Init.NSS = SPI_NSS_HARD_OUTPUT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 0;
    if (HAL_SPI_Init(&hspi2) != HAL_OK)
    {
        SPI_Error_Handler();
    }
    __HAL_SPI_ENABLE(&hspi2);
    PHout(12)=1;PBout(12)=1;
}

static void SPI_Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
     // BSP_OUT04=0; 
  }
  /* USER CODE END Error_Handler */ 
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI2)
  {
    /* USER CODE BEGIN SPI1_MspInit 0 */
    
    
    /* USER CODE END SPI1_MspInit 0 */
    
    
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_DMA1_CLK_ENABLE();
    
    /**SPI1 GPIO Configuration    
    PA4     ------> SPI2_NSS
    PA5     ------> SPI2_SCK
    PA6     ------> SPI2_MISO
    PA7    ------> SPI2_MOSI 
    */
    
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;  
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2; // GPIO_AF5_SPI1
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin  = GPIO_PIN_12;         //SPI_NSS
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin  = GPIO_PIN_12;         //SPI_NSS
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
    
//    /* Peripheral DMA init*/
//    hdma_spi2_tx.Instance = DMA1_Stream4;
//    hdma_spi2_tx.Init.Channel = DMA_CHANNEL_0;
//    hdma_spi2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_spi2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi2_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi2_tx.Init.Mode = DMA_NORMAL;
//    hdma_spi2_tx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi2_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi2_tx) != HAL_OK)
//    {
//        SPI_Error_Handler();
//    }
//
//    __HAL_LINKDMA(hspi,hdmatx,hdma_spi2_tx);
//
//    hdma_spi2_rx.Instance = DMA1_Stream3;
//    hdma_spi2_rx.Init.Channel = DMA_CHANNEL_0;
//    hdma_spi2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_spi2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_spi2_rx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_spi2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_spi2_rx.Init.Mode = DMA_NORMAL;
//    hdma_spi2_rx.Init.Priority = DMA_PRIORITY_LOW;
//    hdma_spi2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//    if (HAL_DMA_Init(&hdma_spi2_rx) != HAL_OK)
//    {
//        SPI_Error_Handler();
//    }
    
//    __HAL_LINKDMA(hspi,hdmarx,hdma_spi2_rx);
    
//    /* DMA interrupt init */
//    /* DMA1_Stream3_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
////    HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
//    
//    BSP_IntVectSet(BSP_INT_ID_DMA1_CH3, DMA1_Stream3_IRQHandler);
//    BSP_IntEn(BSP_INT_ID_DMA1_CH3); //使能中断
//    
//    /* DMA1_Stream4_IRQn interrupt configuration */
//    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
////    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
//    BSP_IntVectSet(BSP_INT_ID_DMA1_CH4, DMA1_Stream4_IRQHandler);
//    BSP_IntEn(BSP_INT_ID_DMA1_CH4); //使能中断 
  }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
//  static DMA_HandleTypeDef hdma_tx;
//  static DMA_HandleTypeDef hdma_rx;
  if(hspi->Instance==SPI2)
  {
      /*##-1- Reset peripherals ##################################################*/ 
      __HAL_RCC_SPI2_FORCE_RESET();
      __HAL_RCC_SPI2_RELEASE_RESET();
      /* USER CODE BEGIN SPI2_MspDeInit 0 */
      
      /* USER CODE END SPI2_MspDeInit 0 */
      /* Peripheral clock disable */
      __HAL_RCC_SPI2_CLK_DISABLE();
      
      /**SPI1 GPIO Configuration    
      PA4     ------> SPI1_NSS
      PA5     ------> SPI1_SCK
      PA6     ------> SPI1_MISO
      PA7    ------> SPI1_MOSI 
      */
      HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_12);
      
      /*##-3- Disable the DMA Streams ############################################*/
      /* De-Initialize the DMA Stream associate to transmission process */
//      HAL_DMA_DeInit(&hdma_spi2_tx);  
//      /* De-Initialize the DMA Stream associate to reception process */
//      HAL_DMA_DeInit(&hdma_spi2_rx);    
//      /*##-4- Disable the NVIC for DMA ###########################################*/
//      HAL_NVIC_DisableIRQ(DMA1_Stream4_IRQn);
//      HAL_NVIC_DisableIRQ(DMA1_Stream3_IRQn);  
//      /*##-5- Disable the NVIC for SPI ###########################################*/
//      HAL_NVIC_DisableIRQ(SPI2_IRQn);
    
  }
  /* USER CODE BEGIN SPI2_MspDeInit 1 */

  /* USER CODE END SPI2_MspDeInit 1 */
}

void SPIInput(uint16_t Tdata)
{  
     while( __HAL_SPI_GET_FLAG(&hspi2,SPI_FLAG_TXE)  == RESET);
    (hspi2.Instance)->DR=Tdata;
     while( __HAL_SPI_GET_FLAG(&hspi2,SPI_FLAG_RXNE)  == RESET);
}
                       
////SPI读写参数
uint16_t SPIWriteWord(uint16_t Tdata)
{ 
      uint16_t receivedbyte = 0;
      SPIInput(Tdata);
      receivedbyte=(hspi2.Instance)->DR;    
      return receivedbyte;
}

uint16_t SPIReadWord(void)
{
     uint16_t receivedbyte = 0;
     SPIInput(0);
     receivedbyte=(hspi2.Instance->DR)&0X00FF;
     return receivedbyte;
}

