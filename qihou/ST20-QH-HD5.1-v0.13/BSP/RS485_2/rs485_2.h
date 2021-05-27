#ifndef __RS485_2_H
#define __RS485_2_H
#include "stm32f10x.h" 


/************************************************
*						ȫ�ֱ����е�extern����
************************************************/
#ifdef	RS485_2_GLOBALS
#define RS485_2_EXT
#else
#define RS485_2_EXT	extern
#endif




#define  RS485_2ND_RX_MAX			250 	//�����������ֽ��� 200



/************************************************
*						ȫ�ֱ�������
************************************************/
RS485_2_EXT u8   RS485_2nd_RX_BUF[RS485_2ND_RX_MAX]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
RS485_2_EXT u16  RS485_2nd_RX_CNT;         		//����״̬���	
RS485_2_EXT u8   FLAG_RS485_2nd_RX;


/************************************************
*					��������
************************************************/
//void RS485_2nd_Init(u32 BaudRate, u8 DataBits, u8 Parity);
void RS485_2nd_SendData(u8 volatile* senddata,u16 len);
void RS485_2nd_SendByte(u8 byte);



#endif


