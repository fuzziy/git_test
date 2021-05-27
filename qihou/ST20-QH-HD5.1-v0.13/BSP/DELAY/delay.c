#include "delay.h"
#include "ucos_ii.h"




/*
**************************************************************************
*									 delay_us		
*
* 说明	：在ucos下实现微秒延时是不太容易的，因为ucos的最快节拍是1ms，
*	  为了实现微秒延时，直接从滴答定时器入手，滴答定时器采用的
*	  时钟位系统时钟的1/8，系统时钟为72M,也就是
*			72000000/8 = 9000000  (9M)
*	  那么一次递减，需要花费1/9000000秒，那么1微秒，需要的递减个数
*	  位(1s=1000ms=1000000us),所以1us = 1/1000000 s
*	 所以滴答定时器走1us时间需要的节拍个数是 9 
* 
* 综上	: 这里定义微秒的因子数为9
**************************************************************************
*/
static u8 fac_us = 9;
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD的值	    	 
	ticks=nus*fac_us; 			//需要的节拍数	  		 
	tcnt=0;
	told=SysTick->VAL;        	//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//时间超过/等于要延迟的时间,则退出.
		}  
	}; 									    
}


void delay_ms(u16 nms)
{
	u8 hour = 0;
	u8 min = 0;
	u8 sec = 0;
	u16 ms = 0;
	
	ms = nms%1000;
	sec = nms/1000%60;
	min = nms/1000/60%60;
	hour = nms/1000/60/60;
	
	OSTimeDlyHMSM(hour, min, sec, ms);
}
