#include "bsp_DACoutput.h"

DAC_HandleTypeDef    DacHandle;
DAC_ChannelConfTypeDef sConfig;

uint32_t DACCH1_DAT,DACCH2_DAT;
uint32_t DACCH1_SecDAT,DACCH2_SecDAT;

void DAC_Error_Handler();
void BSP_DMADAC1_ISR_Handler(void);
void BSP_DMADAC2_ISR_Handler(void);
void TIM6_Config(void);

// 通过控制外部时钟或者定时器控制DMA传输
// 软件模式通过控制SWTRIGR标志位传输
void HAL_DAC_ConfigInit(void)
{
     //TIM6_Config();
     DacHandle.Instance = DAC;
     HAL_DAC_DeInit(&DacHandle);
     
    /*##-1- Initialize the DAC peripheral ######################################*/
    if(HAL_DAC_Init(&DacHandle) != HAL_OK)
    {
      /* Initialization Error */
      DAC_Error_Handler();
    }
    
    //通道一配置
    /*##-1- DAC channel1 Configuration #########################################*/
    sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE; 
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  
    if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL1) != HAL_OK)
    {
      /* Channel configuration Error */
      DAC_Error_Handler();
    }
    
    
    //选择软件触发模式，通道一
    HAL_DAC_Start(&DacHandle,DACx_CHANNEL1);
    HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL1,DAC_ALIGN_12B_R,0X200);
    (DacHandle.Instance)->SWTRIGR = 0x03;
    
    
//    /*##-2- Enable DAC Channel1 and associated DMA #############################*/
//    if(HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL1, (uint32_t*)&DACCH1_DAT, 1 , DAC_ALIGN_12B_R) != HAL_OK)
//    {
//        /* Start DMA Error */
//        DAC_Error_Handler();
//    }
    
    //通道二配置
    /*##-1- DAC channel1 Configuration #########################################*/
    sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  
    if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL2) != HAL_OK)
    {
      /* Channel configuration Error */
      DAC_Error_Handler();
    }    
    
    //选择软件触发模式，通道二
    HAL_DAC_Start(&DacHandle,DACx_CHANNEL2);
    HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL2,DAC_ALIGN_12B_R,0X200);
    (DacHandle.Instance)->SWTRIGR = 0x03;
    
   
//    /*##-2- Enable DAC Channel1 and associated DMA #############################*/
//    if(HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL2, (uint32_t*)&DACCH2_DAT, 1 , DAC_ALIGN_12B_R) != HAL_OK)
//    {
//        /* Start DMA Error */
//        DAC_Error_Handler();
//    }    
    
    //HAL_DACEx_MultiModeStart_DMA(&DacHandle,(uint32_t*)&DACCH2_DAT,1);
    
    //配置波形输出模式
//    /*##-2- DAC channel2 Configuration #########################################*/
//    sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
//    sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;  
//
//    if(HAL_DAC_ConfigChannel(&DacHandle, &sConfig, DACx_CHANNEL1) != HAL_OK)
//    {
//      /* Channel configuration Error */
//      DAC_Error_Handler();
//    }
//    /*##-3- DAC channel2 Triangle Wave generation configuration ################*/
//    if(HAL_DACEx_TriangleWaveGenerate(&DacHandle, DACx_CHANNEL1, DAC_TRIANGLEAMPLITUDE_1023) != HAL_OK)
//    {
//      /* Triangle wave generation Error */
//      DAC_Error_Handler();
//    }
//    
//    /*##-4- Enable DAC Channel1 ################################################*/
//    if(HAL_DAC_Start(&DacHandle, DACx_CHANNEL1) != HAL_OK)
//    {
//      /* Start Error */
//      DAC_Error_Handler();
//    }
//
//    /*##-5- Set DAC channel1 DHR12RD register ################################################*/
//    if(HAL_DAC_SetValue(&DacHandle, DACx_CHANNEL1, DAC_ALIGN_12B_R, 0x100) != HAL_OK)
//    {
//      /* Setting value Error */
//      DAC_Error_Handler();
//    }
    
}

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef          GPIO_InitStruct;
    static DMA_HandleTypeDef  hdma_dac1;
    static DMA_HandleTypeDef  hdma_dac2;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* DAC Periph clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();
    /* Enable GPIO clock ****************************************/
    DACx_CHANNEL1_GPIO_CLK_ENABLE();
    /* DMA1 clock enable */
    DMAx_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/ 
    /* DAC Channel 1 GPIO pin configuration */
    GPIO_InitStruct.Pin =  DACx_CHANNEL1_PIN | DACx_CHANNEL2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
    
    /*##-3- Configure the DMA streams ##########################################*/
    /* Set the parameters to be configured for Channel1*/
//    //通道一
//    hdma_dac1.Instance = DACx_DMA_STREAM1;
//    hdma_dac1.Init.Channel   = DACx_DMA_CHANNEL1;
//    hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_dac1.Init.MemInc    = DMA_MINC_ENABLE;
//    hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_dac1.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
//    hdma_dac1.Init.Mode     = DMA_CIRCULAR;
//    hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;
//    hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
//    hdma_dac1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
//    hdma_dac1.Init.MemBurst = DMA_MBURST_SINGLE;
//    hdma_dac1.Init.PeriphBurst = DMA_PBURST_SINGLE;     
//    HAL_DMA_Init(&hdma_dac1);  
//    
//    //通道二
//    hdma_dac2.Instance = DACx_DMA_STREAM2;
//    hdma_dac2.Init.Channel   = DACx_DMA_CHANNEL2;
//    hdma_dac2.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_dac2.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_dac2.Init.MemInc    = DMA_MINC_ENABLE;
//    hdma_dac2.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_dac2.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
//    hdma_dac2.Init.Mode     = DMA_CIRCULAR;
//    hdma_dac2.Init.Priority = DMA_PRIORITY_HIGH;
//    hdma_dac2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
//    hdma_dac2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
//    hdma_dac2.Init.MemBurst = DMA_MBURST_SINGLE;
//    hdma_dac2.Init.PeriphBurst = DMA_PBURST_SINGLE; 
//    HAL_DMA_Init(&hdma_dac2);  
    
//    /* Associate the initialized DMA handle to the the DAC handle */
//    __HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1);
//    __HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac2);
//    
//    /*##-4- Configure the NVIC for DMA #########################################*/
//    /* Enable the DMA1 Stream5 IRQ Channel */
//    HAL_NVIC_SetPriority(DACx_DMA_IRQn1, 0, 5); 
//    BSP_IntVectSet(BSP_INT_ID_DMA1_CH5, BSP_DMADAC1_ISR_Handler);
//    BSP_IntEn(BSP_INT_ID_DMA1_CH5);    //使能中断
//    
//    HAL_NVIC_SetPriority(DACx_DMA_IRQn2, 0, 6);   
//    BSP_IntVectSet(BSP_INT_ID_DMA1_CH6, BSP_DMADAC2_ISR_Handler);
//    BSP_IntEn(BSP_INT_ID_DMA1_CH6);    //使能中断

//    if(HAL_DMAEx_MultiBufferStart_IT(&hdma_dac1,(uint32_t)(&DACCH1_DAT),(uint32_t)(&((DacHandle.Instance)->DHR12R1)),(uint32_t)(&DACCH1_SecDAT),1) != HAL_OK)
//    {
//      /* Start DMA Error */
//      DAC_Error_Handler();
//    }
//    
//    if(HAL_DMAEx_MultiBufferStart_IT(&hdma_dac2,(uint32_t)(&DACCH2_DAT),(uint32_t)(&((DacHandle.Instance)->DHR12R2)),(uint32_t)(&DACCH2_SecDAT),1) != HAL_OK)
//    {
//      /* Start DMA Error */
//      DAC_Error_Handler();
//    }
}

void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac)
{
  static DMA_HandleTypeDef  hdma_dac1;
  static DMA_HandleTypeDef  hdma_dac2;
  
  /*##-1- Reset peripherals ##################################################*/
  DACx_FORCE_RESET();
  DACx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the DAC Channel1 GPIO pin */
  HAL_GPIO_DeInit(DACx_CHANNEL1_GPIO_PORT, DACx_CHANNEL1_PIN);
  HAL_GPIO_DeInit(DACx_CHANNEL2_GPIO_PORT, DACx_CHANNEL2_PIN);
  /*##-3- Disable the DMA Streams ############################################*/
//  /* De-Initialize the DMA Stream associate to Channel1 */
//  hdma_dac1.Instance = DACx_DMA_STREAM1; 
//  hdma_dac2.Instance = DACx_DMA_STREAM2; 
//  HAL_DMA_DeInit(&hdma_dac1);
//  HAL_DMA_DeInit(&hdma_dac2);  
//  /*##-4- Disable the NVIC for DMA ###########################################*/
//  HAL_NVIC_DisableIRQ(DACx_DMA_IRQn1);
//  HAL_NVIC_DisableIRQ(DACx_DMA_IRQn2);
}

//void BSP_DMADAC1_ISR_Handler(void)
//{
//    HAL_DMA_IRQHandler(DacHandle.DMA_Handle1);
//}
//void BSP_DMADAC2_ISR_Handler(void)
//{
//    HAL_DMA_IRQHandler(DacHandle.DMA_Handle2);
//}

void DAC_Error_Handler(void)
{
  
}

//void TIM6_Config(void)
//{
//  static TIM_HandleTypeDef  htim;
//  TIM_MasterConfigTypeDef sMasterConfig;
//  
//  /*##-1- Configure the TIM peripheral #######################################*/
//  /* Time base configuration */
//  htim.Instance = TIM6;
//  
//  htim.Init.Period = 0x7FF;          
//  htim.Init.Prescaler = 0;       
//  htim.Init.ClockDivision = 0;    
//  htim.Init.CounterMode = TIM_COUNTERMODE_UP; 
//  HAL_TIM_Base_Init(&htim);
//
//  /* TIM6 TRGO selection */
//  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  
//  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
//  
//  /*##-2- Enable TIM peripheral counter ######################################*/
//  HAL_TIM_Base_Start(&htim);
//}
//
//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
//{
//  /* TIM6 Periph clock enable */
//  __HAL_RCC_TIM6_CLK_ENABLE();
//}
//
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
//{
//  
//  /*##-1- Reset peripherals ##################################################*/
//  __HAL_RCC_TIM6_FORCE_RESET();
//  __HAL_RCC_TIM6_RELEASE_RESET();
//}