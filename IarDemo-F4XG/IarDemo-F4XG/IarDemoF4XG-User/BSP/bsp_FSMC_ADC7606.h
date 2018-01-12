#ifndef __BSP_FSMC_ADC7606_H
#define __BSP_FSMC_ADC7606_H

#include  <stm32f4xx.h>
#include <sys.h>

#define AD7606_GIPOG_CS       GPIO_PIN_12
#define AD7606_GPIOH_RST      GPIO_PIN_14
#define AD7606_GIPOI_STBY     GPIO_PIN_1  //in
#define AD7606_GPIOI_RANG     GPIO_PIN_0  
#define AD7606_GPIOH_CONVERT  GPIO_PIN_15
#define AD7606_GPIOD_RD       GPIO_PIN_4
#define AD7606_GPIOH_BUSY     GPIO_PIN_13
#define AD7606_GPIOG_FSTDATA  GPIO_PIN_8
#define AD7606_GPIOA_OS0      GPIO_PIN_15
#define AD7606_GPIOI_OS1      GPIO_PIN_3
#define AD7606_GPIOI_OS2      GPIO_PIN_2

//Êä³öI/O¿ØÖÆ
#define BSP_AD7606_CS         PGout(12)
#define BSP_AD7606_RST        PHout(14)
#define BSP_AD7606_STBY       PIout(1)
#define BSP_AD7606_RANG       PIout(0)
#define BSP_AD7606_CONVERT    PHout(15)
#define BSP_AD7606_RD         PDout(4)

#define BSP_AD7606_BUSY       PHin(13)
#define BSP_AD7606_FSTDATA    PGin(8)

#define BSP_AD7606_OS0        PAout(15)
#define BSP_AD7606_OS1        PIout(3)
#define BSP_AD7606_OS2        PIout(2)

#endif