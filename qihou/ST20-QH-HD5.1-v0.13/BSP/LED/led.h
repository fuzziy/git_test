/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

/*--------------------------------------------------------------
 * RUN LED -- PA6
 *------------------------------------------------------------*/   

#define PORT_LED                  GPIOA
#define PIN_LED                   GPIO_Pin_6


#define PORT2_LED                  GPIOE
#define PIN2_LED                   GPIO_Pin_5

#define LED_TOGGLE                (PORT_LED->ODR ^= PIN_LED)

#define LED2_TOGGLE               (PORT2_LED->ODR ^= PIN2_LED)

void LED_Init(void);


void os_running_led_light(void);

void ErrLED_Ioctl(u8 err_code);

#endif