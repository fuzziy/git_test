
/* ----------------------- System includes --------------------------------*/
#include <port2.h>




void MB2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	//usart2 nvic????
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//????????0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//???????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ??????
	NVIC_Init(&NVIC_InitStructure);	//?????????????????VIC?????	
	
        
}

