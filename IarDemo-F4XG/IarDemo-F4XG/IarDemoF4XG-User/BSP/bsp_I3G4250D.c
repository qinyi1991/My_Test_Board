#include  "bsp_I3G4250D.h"

//Register addr
#define WHO_AM_I_r       0x0F
#define CTRL_REG1_wr     0x20     
#define CTRL_REG2_wr     0x21 
#define CTRL_REG3_wr     0x22 
#define CTRL_REG4_wr     0x23
#define CTRL_REG5_wr     0x24 

//参考
#define REFERENCE_DATACAPTURE_wr    0x25

//温度
#define OUT_TEMP_r                  0x26

//状态
#define STATUS_REG_r                0x26

//输出
#define OUT_X_L_r                   0x28
#define OUT_X_H_r                   0x29
#define OUT_Y_L_r                   0x2A
#define OUT_Y_H_r                   0x2B
#define OUT_Z_L_r                   0x2C               
#define OUT_Z_H_r                   0x2D
//FIFO
#define FIFO_CTRL_REG_wr            0x2E
#define FIFO_SRC_REG_r              0x2F

//中断配置
#define INT1_CFG_wr                    0x30
#define INT1_SRC_r                     0x31
#define INT1_THS_XH_r                  0x32
#define INT1_THS_XL_r                  0x33
#define INT1_THS_YH_r                  0x34
#define INT1_THS_YL_r                  0x35
#define INT1_THS_ZH_r                  0x36
#define INT1_THS_ZL_r                  0x37
#define INT1_DURATION_r                0x38

#define  BSP_SPI2_CS   PBout(12)

//void SPIInput(uint16_t Tdata)
void Inital_BSP_I3G4250D(void)
{
    //使用FIFO的读取方式
       
  
  
}


void Write_Command(uint8_t *data)
{
   
  
  
}


void Read_Data(uint16_t *ptr_data)
{
  
  
}

