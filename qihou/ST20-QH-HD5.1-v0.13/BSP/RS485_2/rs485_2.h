#ifndef __RS485_2_H
#define __RS485_2_H
#include "stm32f10x.h" 


/************************************************
*						全局变量中的extern声明
************************************************/
#ifdef	RS485_2_GLOBALS
#define RS485_2_EXT
#else
#define RS485_2_EXT	extern
#endif




#define  RS485_2ND_RX_MAX			250 	//定义最大接收字节数 200



/************************************************
*						全局变量声明
************************************************/
RS485_2_EXT u8   RS485_2nd_RX_BUF[RS485_2ND_RX_MAX]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
RS485_2_EXT u16  RS485_2nd_RX_CNT;         		//接收状态标记	
RS485_2_EXT u8   FLAG_RS485_2nd_RX;


/************************************************
*					函数声明
************************************************/
//void RS485_2nd_Init(u32 BaudRate, u8 DataBits, u8 Parity);
void RS485_2nd_SendData(u8 volatile* senddata,u16 len);
void RS485_2nd_SendByte(u8 byte);



#endif


