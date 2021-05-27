#include "74hc405x.h"

//S0 - PE4
//S1 - PE3
//S2 - PE2
void HC4051_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO , ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;              
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}


/*
*
* PE2    PE3      PE4   channnel ON
*  S2     S1      S0        
*  0      0       0       Y0 - Z  
*  0      0       1       Y1 - Z  
*  0      1       0       Y2 - Z  
*  0      1       1       Y3 - Z  
*  1      0       0       Y4 - Z  
*  1      0       1       Y5 - Z  
*  1      1       0       Y6 - Z  
*  1      1       1       Y7 - Z  
*
*/
void HC4051_Chn(u8 chn)
{
  switch(chn){
  default:
  case 0:           
       GPIO_ResetBits(GPIOE, GPIO_Pin_4);
       GPIO_ResetBits(GPIOE, GPIO_Pin_3);
       GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    break;   
  case 1:
       GPIO_SetBits(GPIOE, GPIO_Pin_4);
       GPIO_ResetBits(GPIOE, GPIO_Pin_3);
       GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    break;
  case 2:
       GPIO_ResetBits(GPIOE, GPIO_Pin_4);
       GPIO_SetBits(GPIOE, GPIO_Pin_3);
       GPIO_ResetBits(GPIOE, GPIO_Pin_2);
    break;
  case 3:
       GPIO_SetBits(GPIOE, GPIO_Pin_4);
       GPIO_SetBits(GPIOE, GPIO_Pin_3);
       GPIO_ResetBits(GPIOE, GPIO_Pin_2);
      break;
  case 4:
       GPIO_ResetBits(GPIOE, GPIO_Pin_4);
       GPIO_ResetBits(GPIOE, GPIO_Pin_3);
       GPIO_SetBits(GPIOE, GPIO_Pin_2);
      break;
  case 5:
      GPIO_SetBits(GPIOE, GPIO_Pin_4);
       GPIO_ResetBits(GPIOE, GPIO_Pin_3);
       GPIO_SetBits(GPIOE, GPIO_Pin_2);
      break;
  case 6:
      GPIO_ResetBits(GPIOE, GPIO_Pin_4);
       GPIO_SetBits(GPIOE, GPIO_Pin_3);
       GPIO_SetBits(GPIOE, GPIO_Pin_2);
      break;
  case 7:
      GPIO_SetBits(GPIOE, GPIO_Pin_4);
      GPIO_SetBits(GPIOE, GPIO_Pin_3);
      GPIO_SetBits(GPIOE, GPIO_Pin_2);
      break;
  }
}