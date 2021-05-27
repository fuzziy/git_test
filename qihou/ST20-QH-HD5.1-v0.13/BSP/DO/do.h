#ifndef  __DO_H
#define  __DO_H
#include "stm32f10x.h"



void DO_Init(void);
void DO_Ioctl(u8 chn, u16 cmd);
#endif
