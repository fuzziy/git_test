#ifndef __RS485_3_H
#define __RS485_3_H
#include "stm32f10x.h" 


/************************************************
*						ȫ�ֱ����е�extern����
************************************************/
#ifdef	RS485_3_GLOBALS
#define RS485_3_EXT
#else
#define RS485_3_EXT	extern
#endif






/************************************************
*					��������
************************************************/
void RS485_3rd_Init(u32 BaudRate, u8 DataBits, u8 Parity);
void RS485_3rd_SendData(u8 volatile *senddata,u16 len);
void RS485_3rd_SendByte(u8 byte);



#endif


