#include "bsp_EncoderScan.h"

uint32_t  Encoder_left_Var,Encoder_LCylinder_Var;
uint32_t  Encoder_right_Var,Encoder_RCylinder_Var;
uint32_t  Position_Left,Position_Right;
/*
#define  BSP_CLK1    PDout(5)
#define  BSP_DAT4    PBin(7)
*/
void Delay_nop(uint8_t numb)
{
   uint8_t i=0;
   if(numb>0) i=numb;
   else i=1;
   while(i--);
}

//获取二进制码
void Read_Left_SSI_Encode(uint8_t Encoder_N_Bits,uint32_t *ptr1)
{
          uint8_t tclk,Toggle_Flag=0;
          *ptr1=0; 
          Encoder_N_Bits=2*Encoder_N_Bits;
          BSP_CLK1 = 0;             //clk = 0;
          for(tclk = 1;tclk <= Encoder_N_Bits; tclk++){
                if (Toggle_Flag == 1){
                    Toggle_Flag=0;
                    BSP_CLK1 = 0;   //clk = 0;
                    *ptr1 = *ptr1 | BSP_DAT4;
                }else {
                  Toggle_Flag=1;
                  BSP_CLK1 = 1;    //clk = 1;
                  *ptr1 = *ptr1 <<1;
                }
            }
            if (tclk >= Encoder_N_Bits) {
                  tclk=0;
                  BSP_CLK1 = 1;   //clk = 0;
            } 
}

//读取SSI编码器
void Read_Right_SSI_Encode(uint8_t Encoder_N_Bits,uint32_t *ptr1)
{
          uint8_t tclk,Toggle_Flag=0;
          *ptr1=0; 
          Encoder_N_Bits=2*Encoder_N_Bits;
          BSP_CLK2 = 0;             //clk = 0;
          for(tclk = 1;tclk <= Encoder_N_Bits; tclk++){
                if (Toggle_Flag == 1){
                    Toggle_Flag=0;
                    BSP_CLK2 = 0;   //clk = 0;
                    *ptr1 = *ptr1 | BSP_DAT3;
                }else {
                  Toggle_Flag=1;
                  BSP_CLK2 = 1;    //clk = 1;
                  *ptr1 = *ptr1 <<1;
                }
            }
            if (tclk >= Encoder_N_Bits) {
                  tclk=0;
                  BSP_CLK2 = 1;   //clk = 0;
            } 
}

//格雷码解码程序
//Encoder_N_Bits:二进制码的位数
//*ptr：二进制码存储位置指针
void DecodingPrint(uint32_t *ptr,uint8_t Encoder_N_Bits)
{
    int8_t i=0;
    uint32_t temp=0;
    
    temp = (*ptr)&(0x1<<(Encoder_N_Bits-1));
    for(i=1;i<=Encoder_N_Bits;i++){
          temp = temp>>1;
          temp = temp^((*ptr)&(0x10000>>i));
          if(temp>0)
          {
              *ptr = *ptr | temp;
          }
          else
          {
              *ptr = *ptr&(~(0x10000>>i));
          }
    }
}

//Read_Bin12Bits_Encode
void Encoder_interface(uint32_t* Encoder_one,uint32_t* Encoder_two)
{
     *Encoder_one=0;*Encoder_two=0;
     
     Read_Left_SSI_Encode(17,Encoder_one);  //单圈，格雷码，17BITS
     DecodingPrint(Encoder_one,17);
     
     Read_Right_SSI_Encode(24,Encoder_two); //二进制，24Bits， MSB:圈数 ,LSB:单圈分度
     *Encoder_two = (*Encoder_two)&0xfff;
}