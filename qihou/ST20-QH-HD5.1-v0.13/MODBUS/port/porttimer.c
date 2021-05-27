/*
 * FreeModbus Libary: LPC214X Port
 * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "stm32f10x.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	float usTimeoutMS;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  
  TIM_DeInit(TIM2);

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
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
// 	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	return TRUE;
}


void
vMBPortTimersEnable(  )
{	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	TIM_SetCounter(TIM2, 0);
	//TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void
vMBPortTimersDisable(  )
{
	TIM_SetCounter(TIM2, 0);
	//TIM_Cmd(TIM2, DISABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);
}

void
TIMERExpiredISR( void )
{
    (void)pxMBPortCBTimerExpired();

}