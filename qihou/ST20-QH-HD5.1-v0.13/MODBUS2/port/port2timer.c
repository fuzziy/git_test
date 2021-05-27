
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
	3.5个字符时间区分不同的帧，即接收到的两个字符之间时间间隔小于3.5个字符
	时间时认为是同一个帧的，如果间隔大于3.5个字符时间则认为是不同帧的
	在一般的串口通信中，发送1个字符需要：1位起始位，8位数据位，1位校验位(可无),
	1位停止位,总共 1+8+1+1 = 11位，3.5个字符时间就是 3.5 * 11 = 38.5位，
	假如波特率是9600,那么传输1位的时间是1000/9600 = 0.10416667(ms) ,
	这样，3.5个字符时间就大约是 4 ms ,即定时器需要的中断时间
	*/
#if 0 	
	TIM_TimeBaseStructure.TIM_Period = 0x7E54;		  //CLK==24MHz ((1000000000/9600)*11*3.5)/(1000/24) == 0x7e54
	TIM_TimeBaseStructure.TIM_Prescaler = 0x3;
#endif
  // 这个就是预分频系数，7200/72M = 0.0001,即每100us计数值加1
  //10us x 41 = 4.1ms,即5ms中断一次
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
