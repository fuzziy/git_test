#ifndef __AI_SMART_H
#define __AI_SMART_H	 
#include "stm32f10x.h"


typedef struct ai_control_calc_para
{
	float err_seg1;
	float err_seg2;
	float err_seg3;
	float err_seg4;
	float err_change;
	float err_last;
	float err_seg1_out;
	float err_seg2_out;
	float err_seg3_out;
	float err_seg4_out;

} ai_control_calc_para_t;



ai_control_calc_para_t *getAndUpdate_qh_ai_ctrlPara(void);


float ai_control_calc(ai_control_calc_para_t *ai_para, float sv, float pv);

   
#endif
