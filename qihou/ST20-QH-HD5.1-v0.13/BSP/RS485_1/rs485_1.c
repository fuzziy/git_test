/**************************************
*分配给主RS485接口，用于与上位机通信
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
*						全局变量定义
************************************************/
u8 RS485_1st_RX_BUF[RS485_1st_RX_MAX];     //接收缓冲,最大USART_REC_LEN个字节.
u16 RS485_1st_RX_CNT;       //接收状态标记	
u8  FLAG_RS485_1st_RX=0;


  
void RS485_1st_Init(u32 BaudRate, u8 DataBits, u8 Parity)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟以及复用功能时钟
	
	//RS485_RE	  PA.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //PA.11
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//通用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
#if 0 
  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
#endif

   //USART 初始化设置
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
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
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
	USART_SendData(USART1, byte);//向串口1发送数据
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); /*等待发送完成*/
	RS485_1st_RX_ENABLE;
	OS_EXIT_CRITICAL();
}


void RS485_1st_SendData(u8 volatile* senddata,u16 len)
{
	//OS_CPU_SR cpu_sr=0;		//此条命令同ucos-ii中的 OS_CPU_SR cpu_sr=0;  定义一个局部变量
	int i;
	USART1->SR;
	if(len)
	{
	//	OS_ENTER_CRITICAL();
	for(i=0;i<len;i++)
			{
		//		RS485_1st_TX_ENABLE;   //发送使能
				USART_SendData(USART1, senddata[i]);//向串口1发送数据
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	   	//	RS485_1st_RX_ENABLE;  //接收使能 
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

