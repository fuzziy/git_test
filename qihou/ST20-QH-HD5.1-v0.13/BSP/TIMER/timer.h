#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"


void TIM3_Int_Init(u16 arr,u16 psc);
u32 get_running_secs(void);

void reset_running_secs_update_flag(void);

u8 get_running_secs_update_flag(void);

void reset_200ms_update_flag(void);

u8 get_200ms_update_flag(void);

#endif
