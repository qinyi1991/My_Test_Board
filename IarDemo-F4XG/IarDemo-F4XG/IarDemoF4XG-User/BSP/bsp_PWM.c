#include "bsp_PWM.h"

TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim3;
void Error_Handler(void);
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

//*****************TIME5_CH1********************************************************************
void BSP_Timer5_CH1_Inital(void)
{
      TIM_ClockConfigTypeDef sClockSourceConfig;
      TIM_MasterConfigTypeDef sMasterConfig;
      TIM_OC_InitTypeDef sConfigOC;
      
      BSP_PeriphEn(BSP_PERIPH_ID_TIM5);
      
      htim5.Instance = TIM5;
      htim5.Init.Prescaler = 0;
      htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
      htim5.Init.Period = 60000;
      htim5.Init.ClockDivision = 0;
      if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
      {
        Error_Handler();
      }
      
      sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
      if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
      {
        Error_Handler();
      }
      
      if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
      {
        Error_Handler();
      }
      
      sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
      {
        Error_Handler();
      }
      
      //PWM1
      sConfigOC.OCMode = TIM_OCMODE_PWM1;
      sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
      sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
      
     /* Start channel 1 */
      sConfigOC.Pulse = 30000;
      if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
      {
        Error_Handler();
      }
      
      if(HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1) != HAL_OK)
      {
        /* PWM Generation Error */
        Error_Handler();
      }
      HAL_TIM_MspPostInit(&htim5); 
}


//*****************TIME3_CH2 PWM1********************************************************************
void BSP_Timer3_CH2_Inital(void)
{
      TIM_ClockConfigTypeDef sClockSourceConfig;
      TIM_MasterConfigTypeDef sMasterConfig;
      TIM_OC_InitTypeDef sConfigOC;
      
      BSP_PeriphEn(BSP_PERIPH_ID_TIM3);
      
      htim3.Instance = TIM3;
      htim3.Init.Prescaler = 0;
      htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
      htim3.Init.Period = 60000;
      htim3.Init.ClockDivision = 0;
      if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
      {
        Error_Handler();
      }
      
      sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
      if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
      {
        Error_Handler();
      }
      
      if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
      {
        Error_Handler();
      }
      
      sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
      sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
      if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
      {
        Error_Handler();
      }
      
      //PWM1
      sConfigOC.OCMode = TIM_OCMODE_PWM1;
      sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
      sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
      
     /* Start channel 1 */
      sConfigOC.Pulse = 30000;
      if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
      {
        Error_Handler();
      }
      
      if(HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2) != HAL_OK)
      {
        /* PWM Generation Error */
        Error_Handler();
      }
      HAL_TIM_MspPostInit(&htim3); 
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
    
  }
  /* USER CODE END Error_Handler */ 
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
      GPIO_InitTypeDef GPIO_InitStruct;
      if(htim->Instance==TIM5)
      {
        BSP_PeriphEn(BSP_PERIPH_ID_GPIOH);
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
        
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM5;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
      }
      if(htim->Instance==TIM3)
      {
        BSP_PeriphEn(BSP_PERIPH_ID_GPIOA);
        GPIO_InitStruct.Pin = GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
        
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      }      
}

