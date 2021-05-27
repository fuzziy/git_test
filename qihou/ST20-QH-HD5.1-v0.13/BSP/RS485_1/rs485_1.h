#ifndef __RS485_1_H
#define __RS485_1_H
#include "stm32f10x.h" 

/************************************************
*						全局变量中的extern声明
************************************************/
#ifdef	RS485_1ST_GLOBALS
#define RS485_1_EXT
#else
#define RS485_1_EXT	extern
#endif



/************************************************
*							宏定义
************************************************/
#define  RS485_1st_TX_ENABLE  GPIO_SetBits(GPIOA,GPIO_Pin_11)       //发送使能，此处不用加分号(;),因为在引用程序里，会添加分号;
#define  RS485_1st_RX_ENABLE  GPIO_ResetBits(GPIOA,GPIO_Pin_11)    //接收使能
#define  RS485_1st_RX_MAX			250 	//定义最大接收字节数 200



/************************************************
*						全局变量声明
************************************************/
RS485_1_EXT u8   RS485_1st_RX_BUF[RS485_1st_RX_MAX]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
RS485_1_EXT u16  RS485_1st_RX_CNT;         		//接收状态标记	
RS485_1_EXT u8   FLAG_RS485_1st_RX;


/************************************************
*					函数声明
************************************************/
void RS485_1st_Init(u32 BaudRate, u8 DataBits, u8 Parity);
void RS485_1st_SendData(u8 volatile* senddata,u16 len);
void RS485_1st_SendByte(u8 byte);


void RS485_1st_DMAConfig(void);
#endif


