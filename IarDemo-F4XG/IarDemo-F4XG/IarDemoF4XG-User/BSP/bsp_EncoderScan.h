#ifndef __BSP_ENCODERSCAN_H
#define __BSP_ENCODERSCAN_H
#include  <bsp.h>
#include  <stm32f4xx.h>

void Fixture_Sensor_Scan(void *p_arg);
void Encoder_interface(uint32_t* Encoder_one,uint32_t* Encoder_two);

extern  uint32_t Encoder_left_Var,Encoder_LCylinder_Var;
extern  uint32_t Encoder_right_Var,Encoder_RCylinder_Var;
extern  uint32_t  Position_Left,Position_Right;

#endif
