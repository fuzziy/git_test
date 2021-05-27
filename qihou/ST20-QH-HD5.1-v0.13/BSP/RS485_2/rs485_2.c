/**************************************
*�������RS485�ӿڣ���������λ��ͨ��
*CPU_TX-PA2
*CPU_RX-PA3
***************************************/
#define RS485_2_GLOBALS
#include "rs485_2.h"	
#include "os_cpu.h"

#include "port2serial.h"



/************************************************
*						ȫ�ֱ�������
************************************************/
u8 RS485_2nd_RX_BUF[RS485_2ND_RX_MAX];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 RS485_2nd_RX_CNT;       //����״̬���	
u8  FLAG_RS485_2nd_RX=0;


/*
void RS485_2nd_Init(u32 BaudRate, u8 DataBits, u8 Parity)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART2��GPIOAʱ���Լ����ù���ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = BaudRate;
	if(Parity>2)
		Parity = 0;
	switch(Parity)
	{
		case 0:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;
		case 1:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
		case 2:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
	}
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
  if(USART_GetITStatus(USART2, USART_FLAG_PE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_PE);
  }
  
     if(USART_GetITStatus(USART2, USART_FLAG_NE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_NE);
  }
  
    if(USART_GetITStatus(USART2, USART_FLAG_FE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_FE);
  }
  
  if(USART_GetITStatus(USART2, USART_FLAG_ORE) != RESET)
    {
      USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_ORE);
    }
    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{	
            USART_ClearFlag(USART2, USART_IT_RXNE);
            USART_ClearITPendingBit(USART2, USART_IT_RXNE);
            prvvUART2RxISR();
		
	}

	if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
	{
		prvvUART2TxReadyISR();
		USART_ClearITPendingBit(USART2, USART_IT_TXE);
	}
} 

*/

void RS485_2nd_SendByte(u8 byte)
{
	OS_CPU_SR cpu_sr=0;
	USART2->SR;
	OS_ENTER_CRITICAL();
	//RS485_2nd_TX_ENABLE;
	USART_SendData(USART2, byte);//�򴮿�1��������
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); /*�ȴ��������*/
	//RS485_2nd_RX_ENABLE;
	OS_EXIT_CRITICAL();
}


void RS485_2nd_SendData(u8 volatile* senddata,u16 len)
{
 // OS_CPU_SR cpu_sr=0;		//��������ͬucos-ii�е� OS_CPU_SR cpu_sr=0;  ����һ���ֲ�����
  int i;
  USART2->SR;
  if(len)
  {
 //   OS_ENTER_CRITICAL();
    for(i=0;i<len;i++)
    {
		
      USART_SendData(USART2, senddata[i]);//�򴮿�1��������
      while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);
	   	
     }
			
   //   OS_EXIT_CRITICAL();
   }
}



