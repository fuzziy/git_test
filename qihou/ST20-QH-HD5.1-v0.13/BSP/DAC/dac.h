#ifndef __DAC_H
#define __DAC_H	 
#include "stm32f10x.h"

#define ACTUATOR_TYPE_SINGLE_VALVE	0
#define ACTUATOR_TYPE_DOUBLE_VALVE	1
#define ACTUATOR_TYPE_PUMP			2	



								   
void DAC_Config(void);//回环模式初始化		 	 
void DAC_OutmA(u8 chn, float mA);
void DAC_OutV(u16 chn, float voltage);
void DAC_OutElec(u16 chn, float output, u16 type);
u8 DAC_Output(u16 chn, float output, u16 range, u16 type);
void hal_dac_output(u16 actuator_type, u16 dac_signal_type, float value);



#endif
