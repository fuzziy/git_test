/**************************************
*分配给主RS485接口，用于与上位机通信
*CPU_TX-PB10
*CPU_RX-PB11
***************************************/
#define RS485_3_GLOBALS
#include "rs485_3.h"	
#include "os_cpu.h"

#include "port3serial.h"



/************************************************
*						全局变量定义
************************************************/


  
void RS485_3rd_Init(u32 BaudRate, u8 DataBits, u8 Parity)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//使能USART3，GPIOA时钟以及复用功能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//USART3_TX   PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
   
	//USART3_RX	  PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	

   //USART 初始化设置
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
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
}

void USART3_IRQHandler(void)                	//串口1中断服务程序
{
  if(USART_GetITStatus(USART3, USART_FLAG_PE) != RESET)  
  {
      USART_ReceiveData(USART3);
      USART_ClearFlag(USART3, USART_FLAG_PE);
  }
  
  if(USART_GetITStatus(USART3, USART_FLAG_NE) != RESET)  
  {
      USART_ReceiveData(USART3);
      USART_ClearFlag(USART3, USART_FLAG_NE);
  }
  
  if(USART_GetITStatus(USART3, USART_FLAG_FE) != RESET)  
  {
    USART_ReceiveData(USART3);
      USART_ClearFlag(USART3, USART_FLAG_FE);
  }
  
  if(USART_GetITStatus(USART3, USART_FLAG_ORE) != RESET)
  {
      USART_ReceiveData(USART3);
      USART_ClearFlag(USART3, USART_FLAG_ORE);
   }
    
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{	
            USART_ClearFlag(USART3, USART_IT_RXNE);
            USART_ClearITPendingBit(USART3, USART_IT_RXNE);
            prvvUART3RxISR();
		
	}

	if(USART_GetITStatus(USART3, USART_IT_TXE) == SET)
	{
		prvvUART3TxReadyISR();
		USART_ClearITPendingBit(USART3, USART_IT_TXE);
	}
} 


void RS485_3rd_SendByte(u8 byte)
{
	//OS_CPU_SR cpu_sr=0;
	USART3->SR;
	//OS_ENTER_CRITICAL();
	//RS485_3rd_TX_ENABLE;
	USART_SendData(USART3, byte);//向串口1发送数据
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); /*等待发送完成*/
	//RS485_3rd_RX_ENABLE;
	//OS_EXIT_CRITICAL();
}


void RS485_3rd_SendData(u8 volatile *senddata,u16 len)
{
  //OS_CPU_SR cpu_sr=0;		//此条命令同ucos-ii中的 OS_CPU_SR cpu_sr=0;  定义一个局部变量
  int i;
  USART3->SR;
  if(len)
  {
   // OS_ENTER_CRITICAL();
    for(i=0;i<len;i++)
    {
		
      USART_SendData(USART3, senddata[i]);//向串口1发送数据
      while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
	   	
     }
			
     // OS_EXIT_CRITICAL();
   }
}



