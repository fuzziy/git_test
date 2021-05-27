#ifndef __DI_H
#define __DI_H	 
#include "stm32f10x.h"
/*--------------------------------------------------------------
 * DI1 -- PC4
 * DI2 -- PB0
 * DI3 -- PF11
 * DI4 -- PF13
 * DI5 -- PF15
 * DI6 -- PG1
 * DI7 -- PE8
 * DI8 -- PE10
 * DI9 -- PE12
 *------------------------------------------------------------*/   	 
#define  DI1_STA    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)  //DI1
#define  DI2_STA    GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)  //DI2
#define  DI3_STA    GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11)  //DI3
#define  DI4_STA    GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_13)  //DI4
#define  DI5_STA    GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_15)  //DI5
#define  DI6_STA    GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_1)  //DI6
#define  DI7_STA    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8)  //DI6
#define  DI8_STA    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10)  //DI6
#define  DI9_STA    GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12)  //DI6

/*--------------------------------------------------------------
 * ADDR_S1 -- PD8
 * ADDR_S2 -- PB15
 * ADDR_S3 -- PB14
 * ADDR_S4 -- PB13
 * ADDR_S5 -- PB12
 * ADDR_S6 -- PE15
 * ADDR_S7 -- PE14
 * ADDR_S8 -- PE13
 *------------------------------------------------------------*/ 
#define ADDR_S1_STA      GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)        
#define ADDR_S2_STA      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)   
#define ADDR_S3_STA      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)   
#define ADDR_S4_STA      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)   
#define ADDR_S5_STA      GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)   
#define ADDR_S6_STA      GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_15)   
#define ADDR_S7_STA      GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14)   
#define ADDR_S8_STA      GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13)    

void DI_Init(void);  
void DI_Scan(void);

void Addr_sw_init(void);
u8 com1_addr_get(u8 start_addr);
u8 com2_addr_get(u8 start_addr);

#endif

