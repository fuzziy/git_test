#include "stm32f10x.h"

#ifndef _PORT2_TIMER_H
#define _PORT2_TIMER_H



u8      xMB2PortTimersInit( u16 usTim1Timerout50us );


void    vMB2PortTimersEnable( void );

void    vMB2PortTimersDisable( void );

void    TIMER2ExpiredISR(void);


#endif