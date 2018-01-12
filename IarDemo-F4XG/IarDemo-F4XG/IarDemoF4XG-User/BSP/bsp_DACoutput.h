#ifndef __BSP_DACOUTPUT_H
#define __BSP_DACOUTPUT_H

#include  <bsp.h>
#include  <stm32f4xx.h>

#define DACx_CHANNEL1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                 __HAL_RCC_DMA1_CLK_ENABLE()  
     
#define DACx_FORCE_RESET()                __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()              __HAL_RCC_DAC_RELEASE_RESET()

/* Definition for DACx Channel1 Pin */
#define DACx_CHANNEL1_PIN                GPIO_PIN_4
#define DACx_CHANNEL1_GPIO_PORT          GPIOA 
#define DACx_CHANNEL2_PIN                GPIO_PIN_5
#define DACx_CHANNEL2_GPIO_PORT          GPIOA 

/* Definition for DACx's Channel1 */
#define DACx_CHANNEL1                    DAC_CHANNEL_1
#define DACx_CHANNEL2                    DAC_CHANNEL_2
/* Definition for DACx's DMA Channel1 */
#define DACx_DMA_CHANNEL1                DMA_CHANNEL_7
#define DACx_DMA_STREAM1                 DMA1_Stream5

#define DACx_DMA_CHANNEL2                DMA_CHANNEL_7
#define DACx_DMA_STREAM2                 DMA1_Stream6 

/* Definition for DACx's NVIC */
#define DACx_DMA_IRQn1                   DMA1_Stream5_IRQn
#define DACx_DMA_IRQHandler1             DMA1_Stream5_IRQHandler

#define DACx_DMA_IRQn2                   DMA1_Stream6_IRQn
#define DACx_DMA_IRQHandler2             DMA1_Stream6_IRQHandler

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_MspDeInit(DAC_HandleTypeDef* hdac);
void HAL_DAC_ConfigInit(void);
extern DAC_HandleTypeDef DacHandle;
#endif