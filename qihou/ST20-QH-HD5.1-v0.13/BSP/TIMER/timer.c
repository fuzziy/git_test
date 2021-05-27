#include "timer.h"
#include "port.h"
#include "port2timer.h"
#include "port3timer.h"
#include "wrappers.h"

void TIM2_IRQHandler(void)
{
    TIMERExpiredISR();
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


void TIM4_IRQHandler(void)
{
    TIMER2ExpiredISR();
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}

void TIM5_IRQHandler(void)
{
    TIMER3ExpiredISR();
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}



void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序


static u32 g_running_secs = 0;
u32 get_running_secs(void)
{
    return g_running_secs;
}

static u8 g_flag_update_secs = 0;
static void set_running_secs_update_flag(void)
{
    g_flag_update_secs = 1;
}

void reset_running_secs_update_flag(void)
{
    g_flag_update_secs = 0;
}

u8 get_running_secs_update_flag(void)
{
    return g_flag_update_secs;
}



void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){  //检查TIM3更新中断发生与否
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
        
		g_running_secs++;
		set_running_secs_update_flag();
    }
}
