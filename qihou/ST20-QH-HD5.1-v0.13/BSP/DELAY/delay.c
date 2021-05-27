#include "delay.h"
#include "ucos_ii.h"




/*
**************************************************************************
*									 delay_us		
*
* ˵��	����ucos��ʵ��΢����ʱ�ǲ�̫���׵ģ���Ϊucos����������1ms��
*	  Ϊ��ʵ��΢����ʱ��ֱ�Ӵӵδ�ʱ�����֣��δ�ʱ�����õ�
*	  ʱ��λϵͳʱ�ӵ�1/8��ϵͳʱ��Ϊ72M,Ҳ����
*			72000000/8 = 9000000  (9M)
*	  ��ôһ�εݼ�����Ҫ����1/9000000�룬��ô1΢�룬��Ҫ�ĵݼ�����
*	  λ(1s=1000ms=1000000us),����1us = 1/1000000 s
*	 ���Եδ�ʱ����1usʱ����Ҫ�Ľ��ĸ����� 9 
* 
* ����	: ���ﶨ��΢���������Ϊ9
**************************************************************************
*/
static u8 fac_us = 9;
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;	//LOAD��ֵ	    	 
	ticks=nus*fac_us; 			//��Ҫ�Ľ�����	  		 
	tcnt=0;
	told=SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
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
