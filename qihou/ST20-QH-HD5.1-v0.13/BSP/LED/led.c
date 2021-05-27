#include "led.h"

/*--------------------------------------------------------------
 * RUN LED -- PA6
 * STAT1 LED -- PG0
 * STAT2 LED -- PE7
 * STAT3 LED -- PE9
 * STAT4 LED -- PE11
 *------------------------------------------------------------*/  

void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;                  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;              
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11;
  GPIO_Init(GPIOE, &GPIO_InitStructure);      

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_Init(GPIOG, &GPIO_InitStructure); 

  
  GPIO_SetBits(GPIOA, GPIO_Pin_6);
  GPIO_SetBits(GPIOE, GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11);
  GPIO_SetBits(GPIOG, GPIO_Pin_0);
}


void RunLed_Ioctl(u8 cmd)
{
  if( cmd ){       
    GPIO_ResetBits(GPIOA, GPIO_Pin_6);
  }else{
    GPIO_SetBits(GPIOA, GPIO_Pin_6);
  }   
}

void os_running_led_light(void)
{
  static u8 flag_state = 0;
  flag_state = !flag_state;
  RunLed_Ioctl( flag_state );
}


/*--------------------------------------------------------------
 * STAT1 LED -- PG0
 * STAT2 LED -- PE7
 * STAT3 LED -- PE9
 * STAT4 LED -- PE11
 *------------------------------------------------------------*/   
void ErrLED_Ioctl(u8 err_code)
{
  u8 tmp;
  //state1 led
  tmp = err_code&0x01;
  if(tmp > 0){
    GPIO_ResetBits(GPIOG, GPIO_Pin_0);
  }else{
    GPIO_SetBits(GPIOG, GPIO_Pin_0);
  }

  //state2 led
  tmp = err_code&0x02;
   if(tmp > 0){
    GPIO_ResetBits(GPIOE, GPIO_Pin_7);
  }else{
    GPIO_SetBits(GPIOE, GPIO_Pin_7);
  }

  //state3 led
  tmp = err_code&0x04;
   if(tmp > 0){
    GPIO_ResetBits(GPIOE, GPIO_Pin_9);
  }else{
    GPIO_SetBits(GPIOE, GPIO_Pin_9);
  }

  //state4 led
  tmp = err_code&0x08;
   if(tmp > 0){
    GPIO_ResetBits(GPIOE, GPIO_Pin_11);
  }else{
    GPIO_SetBits(GPIOE, GPIO_Pin_11);
  }
}

