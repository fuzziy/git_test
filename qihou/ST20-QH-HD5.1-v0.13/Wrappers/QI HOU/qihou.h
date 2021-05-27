#ifndef __QIHOU_H
#define __QIHOU_H	 
#include "stm32f10x.h"
/*--------- 2020 new -----------------------------------------*/
#define QH_MODE_MANUAL               0     //  手动模式
#define QH_MODE_SET                  1     //  设定模式
#define QH_MODE_CALC                 2	   //  计算模式
#define QH_MODE_AI                   3     // AI mode


float get_qh_target_temp(u16 mode);
float get_qh_segment_offset(void);
float get_qh_holiday_offset(void);
float get_qh_solar_offset(void);
float qh_target_temp_hl_cmp(float target);
float get_outdoor_temp(void);

float get_qh_feedback(void);

float get_water_feedback(void);

float get_outdoor_temp_loc(void);

int update_qh_feedback_valueAndRegs(u8 current_adc_chn, float current_adc_value);
void  qh_main_task(void *p_arg);


#endif