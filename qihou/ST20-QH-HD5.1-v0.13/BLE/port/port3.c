
/* ----------------------- System includes --------------------------------*/
#include <port3.h>




void MB3_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//USART3 nvic????
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//????????0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;		//???????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ??????
	NVIC_Init(&NVIC_InitStructure);	//?????????????????VIC?????	
	//tim4 nvic????
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
	NVIC_Init(&NVIC_InitStructure);	
}

