#ifndef __RX8025_H
#define __RX8025_H
#include "stm32f10x.h"
#include "bsp.h"

//IO�������ã�ͨ���Ĵ���ֱ���趨;
//GPIOx_CRL�Ƕ�GPIOx�ĵ�8���˿����ã���PB0~PB7;GPIOx_CRH�ǶԸ�8���˿����ã���PB8~PB15
//I2C1��SDA��Ӧ�Ķ˿���PB7
/*
// SCL -- PB5
// SDA -- PB8
#define RX8025_SDA_IN()  {GPIOB->CRH &= 0xFFFFFFF0;GPIOB->CRH |= 0x00000008;}
#define RX8025_SDA_OUT() {GPIOB->CRH &= 0xFFFFFFF0;GPIOB->CRH |= 0x00000003;}
	  

#define RX8025_READ_SDA    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)//����SDA 

#define RX8025_SCL_1     {GPIO_SetBits(GPIOB, GPIO_Pin_5);}
#define RX8025_SCL_0     {GPIO_ResetBits(GPIOB, GPIO_Pin_5);}

#define RX8025_SDA_1     {GPIO_SetBits(GPIOB, GPIO_Pin_8);}
#define RX8025_SDA_0     {GPIO_ResetBits(GPIOB,GPIO_Pin_8);}

*/



u8 get_rx8025t_time(void);

u8 rx8025t_init(void);
u8 rx8025t_set_time(rt_time_t *t);

#endif
