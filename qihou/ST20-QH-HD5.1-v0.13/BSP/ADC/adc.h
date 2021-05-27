#ifndef __ADC_H
#define __ADC_H
#include <stm32f10x.h>


#define AI_INPUT_MA			0	// AI input type is 4~20 mA
#define AI_INPUT_NORMAL_VOL		1	// AI input type is normal voltage
#define AI_INPUT_OHM2VOL		2	// AI input type is ohm convert to voltage


float get_AI_variable_result(u8 ai_chn);

float get_qh_feedback(void);
float get_water_feedback(void);
float get_outdoor_temp_loc(void);


#endif