#ifndef __WRAPPERS_H
#define __WRAPPERS_H

#include "stm32f10x.h"

#include "qihou_para.h"
#include "qihou_modbus.h"


typedef void ( *pAppMainCtrl ) (void *p_arg);

extern pAppMainCtrl AppMainCtrl;


void wrapper_para_init(void);
void wrapper_main_task_init(void);
void wrapper_update_proj_dataAndRegs(u8 current_adc_chn, float current_adc_value);






#endif