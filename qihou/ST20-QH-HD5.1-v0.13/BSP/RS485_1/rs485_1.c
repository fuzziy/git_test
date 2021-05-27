/**************************************
*�������RS485�ӿڣ���������λ��ͨ��
*CPU_TX-PA9
*CPU_RX-PA10
*CPU_RE-PA11
***************************************/
#define RS485_1ST_GLOBALS
#include <stdio.h>
#include "rs485_1.h"	
#include "stm32f10x_dma.h"
#include "os_cpu.h"
#include "port.h"




/************************************************
*						ȫ�ֱ�������
************************************************/
u8 RS485_1st_RX_BUF[RS485_1st_RX_MAX];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 RS485_1st_RX_CNT;       //����״̬���	
u8  FLAG_RS485_1st_RX=0;


  
void RS485_1st_Init(u32 BaudRate, u8 DataBits, u8 Parity)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ���Լ����ù���ʱ��
	
	//RS485_RE	  PA.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA.11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//ͨ���������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
#if 0 
  //Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
#endif

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;
	if(Parity>2)
		Parity = 0;
	switch(Parity)
	{
		case NONE:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;
		case ODD:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
		case EVEN:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
                        
                        if(DataBits == 7)
                          USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                        else
                          USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
	}
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
  if(USART_GetITStatus(USART1, USART_FLAG_PE) != RESET)  
  {
    USART_ReceiveData(USART1);
      USART_ClearFlag(USART1, USART_FLAG_PE);
  }
  
    if(USART_GetITStatus(USART1, USART_FLAG_NE) != RESET)  
  {
    USART_ReceiveData(USART1);
      USART_ClearFlag(USART1, USART_FLAG_NE);
  }
   if(USART_GetITStatus(USART1, USART_FLAG_FE) != RESET)  
  {
    USART_ReceiveData(USART1);
      USART_ClearFlag(USART1, USART_FLAG_FE);
  }
  
  if(USART_GetITStatus(USART1, USART_FLAG_ORE) != RESET)
    {
      USART_ReceiveData(USART1);
      USART_ClearFlag(USART1, USART_FLAG_ORE);
    }
  
  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{		
          USART_ClearFlag(USART1, USART_IT_RXNE);
          USART_ClearITPendingBit(USART1, USART_IT_RXNE);
          prvvUARTRxISR();
	}

	if(USART_GetITStatus(USART1, USART_IT_TXE) == SET)
	{
		//prvvUARTTxReadyISR();
		USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}
} 


void RS485_1st_SendByte(u8 byte)
{
	OS_CPU_SR cpu_sr=0;
	USART1->SR;
	OS_ENTER_CRITICAL();
	RS485_1st_TX_ENABLE;
	USART_SendData(USART1, byte);//�򴮿�1��������
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); /*�ȴ��������*/
	RS485_1st_RX_ENABLE;
	OS_EXIT_CRITICAL();
}


void RS485_1st_SendData(u8 volatile* senddata,u16 len)
{
	//OS_CPU_SR cpu_sr=0;		//��������ͬucos-ii�е� OS_CPU_SR cpu_sr=0;  ����һ���ֲ�����
	int i;
	USART1->SR;
	if(len)
	{
	//	OS_ENTER_CRITICAL();
	for(i=0;i<len;i++)
			{
		//		RS485_1st_TX_ENABLE;   //����ʹ��
				USART_SendData(USART1, senddata[i]);//�򴮿�1��������
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	   	//	RS485_1st_RX_ENABLE;  //����ʹ�� 
      }
			
	//	OS_EXIT_CRITICAL();
	}
}

int fputc(int ch, FILE *f)
{
	//send one byte with usart1
	USART_SendData(USART1, (uint8_t)ch);
	//waiting
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return (ch); 
}

