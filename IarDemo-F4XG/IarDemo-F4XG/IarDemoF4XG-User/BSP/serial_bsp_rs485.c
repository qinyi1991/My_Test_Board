#include "serial_bsp_rs485.h"

UART_HandleTypeDef RS485Handle;
uint8_t RFID_Buf[RFID_BUF_SIZE];

CPU_INT08U Get_Crc8(CPU_INT08U *ptr,CPU_INT08U len);
void Decoding_Data(CPU_INT08U *ptr,CPU_INT08U len);

void  BSP_RS485Port_Init (CPU_INT32U  baud_rate)
{
        /* ----------------- INIT USART STRUCT ---------------- */
        RS485Handle.Instance          = USART6;
        
        RS485Handle.Init.BaudRate     = baud_rate;
        RS485Handle.Init.WordLength   = UART_WORDLENGTH_8B;
        RS485Handle.Init.StopBits     = UART_STOPBITS_1;
        RS485Handle.Init.Parity       = UART_PARITY_NONE;
        RS485Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        RS485Handle.Init.Mode         = UART_MODE_TX_RX;
        RS485Handle.Init.OverSampling = UART_OVERSAMPLING_8;
        
        /* ------------------ SETUP USART6-------------------- */
        if(HAL_UART_Init(&RS485Handle) != HAL_OK){
        }
        RS485_EN=0; //打開接收
        if(HAL_UART_Receive_IT(&RS485Handle, (uint8_t*)RFID_Buf, RFID_BUF_SIZE)!= HAL_OK){
         
        }//使能接收中断  
        
}

void RS485Processing()
{
    CPU_INT08U Data_crc;
//    1，检测数据位数是否正确
//    2，检测帧头是否正确
//    3，进行异或校验
//    4，检测帧尾是否和校验位一致
//    5，复制数据位到ID卡号区，并显示在触控屏上。
//    6，数据位为12个字节
    switch(1)
    {
        case 1:{if(RFID_Buf[0] != 0xAA) return;} //帧头
        
        // case 2:{if(RFID_Buf[1] != 0xBB) return;} //随机数1
        // case 3:{if(RFID_Buf[2] != 0xCC) return;} //随机数2
        
        //if(RFID_Buf[10],用户数据位占8个字节，数据总位11字节
        case 4:{
            Data_crc     = Get_Crc8(&RFID_Buf[0],11);
            if(Data_crc != RFID_Buf[11]) {return;}  
        }    
        //CRC8校验数据位
        case 5:{ Decoding_Data(&RFID_Buf[3],8);}   //解密
        
       
    }
  
}

//cec8数据校验
CPU_INT08U Get_Crc8(CPU_INT08U *ptr,CPU_INT08U len)
{
	CPU_INT08U crc;
	CPU_INT08U i;
	crc=0;
	while(len--)
	{
		crc^=*ptr++;
		for(i=0;i<8;i++)
		{
			if(crc&0x01)crc=(crc>>1)^0x8C;
			else crc >>= 1;
		}
	}
	return crc;
}

//解密程序
void Decoding_Data(CPU_INT08U *ptr,CPU_INT08U len)
{
      CPU_INT16U Data;
      CPU_INT08U i;
      for(i=0;i<len;i++)
      {
              Data=RFID_Buf[2]^(*ptr++);
              Data=Data^RFID_Buf[1];
              RFID_Buf[3+i]=Data;
              Data=0;
      }
      
  
}

void Clear_RFIDBuf(void)
{
    uint8_t  i;
    for(i=0;i<RFID_BUF_SIZE;i++){RFID_Buf[i]=0;}
  
  
  
}