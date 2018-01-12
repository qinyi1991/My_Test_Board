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

//片选区
#define  BSP_SPI2_CS   PHout(12)

//写命令
#define  WR_DEVICE  0x8000        //Write:0, Read:1
//地址保持
#define  DEVICE_MS_ADDR  0x4000   //In multi hold:0, increase:1
//FIFO操作
 

//void SPIWriteWord(uint16_t Tdata)
void Inital_BSP_I3G4250D(void)
{
    static  uint16_t Ack_data;
    BSP_SPI2_CS=0;
    //输出带宽
    Write_Command_To_Device(0x88,0x20,&Ack_data); 
    if(Ack_data == 0x88)
    {
   
    }
    
    
    
  
}


//单次写
void Write_Command_To_Device(uint8_t Re_Data,uint8_t Addr,uint16_t* Receive_data)
{
    uint16_t Temp_Data=0;

    Temp_Data = (uint16_t)(Re_Data)|((uint16_t)Addr<<8);
    *Receive_data = SPIWriteWord( Temp_Data);

    
}

//单次读
void Read_Data_From_Device(uint8_t Addr,uint16_t* Receive_data)
{
    uint16_t Temp_Data=0;
    Temp_Data = WR_DEVICE|((uint16_t)Addr<<8);
    *Receive_data = SPIWriteWord( Temp_Data);
}

//混合读
void Multiple_Read_Datas(uint8_t* Ptr_Data,uint8_t Addr)
{
  
  
}

//混合写
void Multiple_Write_Datas(uint8_t* Ptr_Data,uint8_t Addr)
{
  
  
}
