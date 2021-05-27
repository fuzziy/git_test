#include "iwdg.h"

//�������Ź����ʱ��Tout = 4*2^prer*rlr/40
void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}


//PC7
void sp706s_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   
	GPIO_Init(GPIOC, &GPIO_InitStructure);				
}


static u8 sp706s_wdi_state = 0;
void sp706s_feed(void)
{
	if(sp706s_wdi_state){
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		sp706s_wdi_state = 0;
	}else{
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		sp706s_wdi_state = 1;
	}
}


void feed_hd_soft_dog(void)
{
	sp706s_feed();
	IWDG_Feed();
}