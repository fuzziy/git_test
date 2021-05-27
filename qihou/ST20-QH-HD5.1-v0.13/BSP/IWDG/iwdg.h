#ifndef __IWDG_H
#define __IWDG_H
#include "stm32f10x.h"

void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);

void sp706s_init(void);
void sp706s_feed(void);

void feed_hd_soft_dog(void);


#endif
