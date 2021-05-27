#include "stm32f10x.h"

#ifndef _PORT3_TIMER_H
#define _PORT3_TIMER_H



u8      xMB3PortTimersInit( u16 usTim1Timerout50us );


void    vMB3PortTimersEnable( void );

void    vMB3PortTimersDisable( void );

void    TIMER3ExpiredISR(void);


#endif