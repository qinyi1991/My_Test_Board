#ifndef  MODBUS_H
#define  MODBUS_H

//#include <includes.h>	/* 该文件包含了必需的.h文件 */
#include <modus_Controltargetconf.h>
#include <bsp.h>

//线圈和寄存器地址设定
//u16为参考
#define MAX_REG_ADR         251     //0-200
#define MAX_KEEPREG_ADR     100     //0-100 保持寄存器
//u8为参考
#define MAX_COIL_ADR        800     //0-800
#define MAX_KEEPCOIL_ADR    400     //0-400 保持线圈
//-----------------------------------------------------------------
//	Modbus eeprom
//-----------------------------------------------------------------
#define   ModbusCoilAdd       400	   //400-800
#define   ModbusRegAdd        0             //0-400
#define   OperateAdd(a,b)     (a+b)

//地址0不使用
#define   ModbusAdd(n)   ((n*2)-1)  

typedef  struct 
{
	CPU_INT16U       Add;
	CPU_INT16U       num;

} Coil_Dis;

typedef  struct 
{
	CPU_INT16U   Add;
	CPU_INT16U   num;

} Regis_Dis;

extern CPU_INT08U	receCount;	            //接收到的字节个
extern CPU_INT08U  sendBuf[100],receBuf[100];   //发送接收缓冲
extern CPU_INT08U  redrxBuf[20];
extern CPU_INT08U	redrxCount;	            //接收到的字节个

extern CPU_INT08U	receTimeOut;                //接收超时

extern CPU_INT08U  CoilData[MAX_COIL_ADR];              //modbus 线圈状态   1--400 存入EEPRAM
extern CPU_INT16S  RegData[MAX_REG_ADR];	             //modbus 寄存器状态 1-200  存入EEPRAM

extern Coil_Dis   Coil;
extern Regis_Dis  Rdis;
/*======================================================================================================*/

void beginSend(CPU_INT16U sendCount);
void checkComm0Modbus(void);
void readCoil(void);
void readRegisters(void);
void forceSingleCoil(void);
void presetSingleRegister(void);
void presetMultipleRegisters(void);
void forceMultipleCoils(void);
CPU_INT16U SaveRegisterToEEPROM(CPU_INT16U Addr);
CPU_INT16U Write_4byte(void *Add,void *d);

#endif



