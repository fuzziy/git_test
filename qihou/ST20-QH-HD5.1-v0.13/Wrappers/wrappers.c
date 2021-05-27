#include "wrappers.h"
#include "qihou.h"


pAppMainCtrl AppMainCtrl;



void wrapper_update_proj_dataAndRegs(u8 current_adc_chn, float current_adc_value)
{
   update_qh_feedback_valueAndRegs(current_adc_chn, current_adc_value);
}


void wrapper_para_init(void)
{
    qh_para_init();
}

void wrapper_main_task_init(void)
{
    AppMainCtrl = qh_main_task;
}





