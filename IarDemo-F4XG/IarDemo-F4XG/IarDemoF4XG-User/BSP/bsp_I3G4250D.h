#ifndef __BSP_I3G4250D_H
#define __BSP_I3G4250D_H

#include  <stm32f4xx.h>
#include <sys.h>
#include "bsp_SPIbus.h"
void Read_Data_From_Device(uint8_t Addr,uint16_t* Receive_data);
void Write_Command_To_Device(uint8_t Re_Data,uint8_t Addr,uint16_t* Receive_data);
void Inital_BSP_I3G4250D(void);
#endif