#ifndef __BSP_SPIBUS_H
#define __BSP_SPIBUS_H

#include  <stm32f4xx.h>
#include <sys.h>

void BSP_SPI2_Init(void);
void SPIInput(uint16_t Tdata);

uint16_t SPIWriteWord(uint16_t Tdata);
uint16_t SPIReadWord(void);

#endif
