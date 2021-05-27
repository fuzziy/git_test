
/* ----------------------- Platform includes --------------------------------*/
#include "port2timer.h"




extern          u8( *pxMB2PortCBTimerExpired ) ( void );

/* ----------------------- Start implementation -----------------------------*/
u8
xMB2PortTimersInit( u16 usTim1Timerout50us )
{
	float usTimeoutMS;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  TIM_DeInit(TIM4);

	/*
	3.5���ַ�ʱ�����ֲ�ͬ��֡�������յ��������ַ�֮��ʱ����С��3.5���ַ�
	ʱ��ʱ��Ϊ��ͬһ��֡�ģ�����������3.5���ַ�ʱ������Ϊ�ǲ�ͬ֡��
	��һ��Ĵ���ͨ���У�����1���ַ���Ҫ��1λ��ʼλ��8λ����λ��1λУ��λ(����),
	1λֹͣλ,�ܹ� 1+8+1+1 = 11λ��3.5���ַ�ʱ����� 3.5 * 11 = 38.5λ��
	���粨������9600,��ô����1λ��ʱ����1000/9600 = 0.10416667(ms) ,
	������3.5���ַ�ʱ��ʹ�Լ�� 4 ms ,����ʱ����Ҫ���ж�ʱ��
	*/
#if 0 	
	TIM_TimeBaseStructure.TIM_Period = 0x7E54;		  //CLK==24MHz ((1000000000/9600)*11*3.5)/(1000/24) == 0x7e54
	TIM_TimeBaseStructure.TIM_Prescaler = 0x3;
#endif
  // �������Ԥ��Ƶϵ����7200/72M = 0.0001,��ÿ100us����ֵ��1
  //10us x 41 = 4.1ms,��5ms�ж�һ��
	usTimeoutMS = usTim1Timerout50us;
	usTimeoutMS = (usTimeoutMS + 10 )/2;
	TIM_TimeBaseStructure.TIM_Period = ( u16 ) usTimeoutMS;		//41;
	TIM_TimeBaseStructure.TIM_Prescaler = (7200 - 1);	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
// 	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	return 1;
}


void
vMB2PortTimersEnable( void )
{	
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_SetCounter(TIM4, 0);
	//TIM_Cmd(TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

void
vMB2PortTimersDisable( void )
{
	TIM_SetCounter(TIM4, 0);
	//TIM_Cmd(TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
}

void
TIMER2ExpiredISR( void )
{
    (void)pxMB2PortCBTimerExpired();

}
