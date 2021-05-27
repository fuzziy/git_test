#include "do.h"


/*--------------------------------------------------------------
 * DO1 -- PD1
 * DO2 -- PD0
 * DO3 -- PC12
 * DO4 -- PC11
 * DO5 -- PC10
 * 
 * DO1 LED -- PA7
 * DO2 LED -- PC5
 * DO3 LED -- PB1
 * DO4 LED -- PF12
 * DO5 LED -- PF14
 *------------------------------------------------------------*/  

void DO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|
                  RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|
                  RCC_APB2Periph_GPIOF, ENABLE);
  // PD0-1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
  // PC5-10-11-12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
  // PA7
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
  // PB1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  // PF12-14
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14;		
  GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
  // init set
  DO_Ioctl(1, 0);
  DO_Ioctl(2, 0);
  DO_Ioctl(3, 0);
  DO_Ioctl(4, 0);
  DO_Ioctl(5, 0);
}


/*--------------------------------------------------------------
 * DO1 -- PD1
 * DO2 -- PD0
 * DO3 -- PC12
 * DO4 -- PC11
 * DO5 -- PC10
 * 
 * DO1 LED -- PA7
 * DO2 LED -- PC5
 * DO3 LED -- PB1
 * DO4 LED -- PF12
 * DO5 LED -- PF14
 *------------------------------------------------------------*/  
void DO_Ioctl(u8 chn, u16 cmd)
{
  switch( chn ){
    case 1:
      if(cmd){
        GPIO_ResetBits(GPIOD, GPIO_Pin_1);
        GPIO_ResetBits(GPIOA, GPIO_Pin_7);         
      }else{
        GPIO_SetBits(GPIOD, GPIO_Pin_1);
        GPIO_SetBits(GPIOA,GPIO_Pin_7);           
      }
      break;
    case 2:
      if(cmd){
        GPIO_ResetBits(GPIOD, GPIO_Pin_0);
        GPIO_ResetBits(GPIOC, GPIO_Pin_5);         
      }else{
        GPIO_SetBits(GPIOD, GPIO_Pin_0);
        GPIO_SetBits(GPIOC,GPIO_Pin_5);           
      }
      break;
    case 3:
      if(cmd){
        GPIO_ResetBits(GPIOC, GPIO_Pin_12);
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);         
      }else{
        GPIO_SetBits(GPIOC, GPIO_Pin_12);
        GPIO_SetBits(GPIOB,GPIO_Pin_1);           
      }
      break;   
    case 4:
      if(cmd){
        GPIO_ResetBits(GPIOC, GPIO_Pin_11);
        GPIO_ResetBits(GPIOF, GPIO_Pin_12);         
      }else{
        GPIO_SetBits(GPIOC, GPIO_Pin_11);
        GPIO_SetBits(GPIOF, GPIO_Pin_12);           
      }
      break;
    case 5:
      if(cmd){
        GPIO_ResetBits(GPIOC, GPIO_Pin_10);
        GPIO_ResetBits(GPIOF, GPIO_Pin_14);         
      }else{
        GPIO_SetBits(GPIOC, GPIO_Pin_10);
        GPIO_SetBits(GPIOF, GPIO_Pin_14);           
      }
      break;
  }
}

