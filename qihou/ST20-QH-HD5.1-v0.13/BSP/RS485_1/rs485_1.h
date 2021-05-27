#ifndef __RS485_1_H
#define __RS485_1_H
#include "stm32f10x.h" 

/************************************************
*						ȫ�ֱ����е�extern����
************************************************/
#ifdef	RS485_1ST_GLOBALS
#define RS485_1_EXT
#else
#define RS485_1_EXT	extern
#endif



/************************************************
*							�궨��
************************************************/
#define  RS485_1st_TX_ENABLE  GPIO_SetBits(GPIOA,GPIO_Pin_11)       //����ʹ�ܣ��˴����üӷֺ�(;),��Ϊ�����ó��������ӷֺ�;
#define  RS485_1st_RX_ENABLE  GPIO_ResetBits(GPIOA,GPIO_Pin_11)    //����ʹ��
#define  RS485_1st_RX_MAX			250 	//�����������ֽ��� 200



/************************************************
*						ȫ�ֱ�������
************************************************/
RS485_1_EXT u8   RS485_1st_RX_BUF[RS485_1st_RX_MAX]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
RS485_1_EXT u16  RS485_1st_RX_CNT;         		//����״̬���	
RS485_1_EXT u8   FLAG_RS485_1st_RX;


/************************************************
*					��������
************************************************/
void RS485_1st_Init(u32 BaudRate, u8 DataBits, u8 Parity);
void RS485_1st_SendData(u8 volatile* senddata,u16 len);
void RS485_1st_SendByte(u8 byte);


void RS485_1st_DMAConfig(void);
#endif


