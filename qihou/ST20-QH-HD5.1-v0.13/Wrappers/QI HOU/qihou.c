#include "qihou.h"
#include "my_modbus.h"
#include "calculate.h"
#include "adc.h"
#include "dac.h"
#include <stdint.h>
#include <math.h>
#include "ai.h"
#include "ucos_ii.h"
#include "timer.h"

/*------------------------------ 2020 new function --------------------------------------------*/
static float get_outdoor_temp_rem(void)
{
  return Words2Float(&usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_REM], 3412);
}

static float get_feel_temp(void)
{
  return Words2Float(&usRegHoldingBuf[MB_INDEX_FEEL_TEMP], 3412);
}

float get_outdoor_temp(void)
{
  float k;
  float feel_temp;
  float outdoor_temp_rem;
  if(usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_SW] > 0){
    return get_outdoor_temp_loc();
  }else{
    outdoor_temp_rem = get_outdoor_temp_rem();
    if(usRegHoldingBuf[MB_INDEX_MODE] == QH_MODE_AI){
      feel_temp = get_feel_temp();
      k = Words2Float(&usRegHoldingBuf[MB_INDEX_FEEL_TEMP_K], 3412);
      return (outdoor_temp_rem*k + (1 - k)*feel_temp);
    }
    return outdoor_temp_rem;
  }
}

static float get_set_mode_target(void)
{
  return Words2Float(&usRegHoldingBuf[MB_INDEX_SET_TEMP], 3412);
}

static float get_cacl_mode_target(void)
{
  float res = 0.0;
  float outdoor_temp;
  float start_temp;
  float k1, k2, k3, k4;
  outdoor_temp = get_outdoor_temp();
  start_temp = Words2Float(&usRegHoldingBuf[MB_INDEX_START_TEMP], 3412);
  k1 = Words2Float(&usRegHoldingBuf[MB_INDEX_CURVATURE1], 3412);
  k2 = Words2Float(&usRegHoldingBuf[MB_INDEX_CURVATURE2], 3412);
  k3 = Words2Float(&usRegHoldingBuf[MB_INDEX_CURVATURE3], 3412);
  k4 = Words2Float(&usRegHoldingBuf[MB_INDEX_CURVATURE4], 3412);
  if(outdoor_temp < -8.0){
    res = ( start_temp + k2*8.0 ) - k1*( outdoor_temp + 8.0);
    return res;
  }

  if(FloatIntervalJudge(outdoor_temp, -8.0, 0.0)){
    res = start_temp - k2*outdoor_temp;
    return res;
  }

  if(FloatIntervalJudge(outdoor_temp, 0, 8.0)){
    res = start_temp - k3*outdoor_temp;
    return res;
  }

  res = ( start_temp - k3*8) - k4*(outdoor_temp - 8.0);
  return res;
}


#define OUTDOOR_TEMP_SEG_MAX    56
static int16_t outdoor_temp_seg[OUTDOOR_TEMP_SEG_MAX] = {
-40, -39, -38, -37, -36, -35, -34, -33, -32, -31,       // 0 - 9
-30, -29, -28, -27, -26, -25, -24, -23, -22, -21,       // 10 - 19
-20, -19, -18, -17, -16, -15, -14, -13, -12, -11,       // 20 - 29
-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 	        // 30 - 39
0, 1, 2, 3, 4, 5, 6, 7, 8, 9,		                // 40 - 49
10, 11, 12, 13, 14, 15			                // 50 - 55
};

static float get_AI_mode_target(void)
{
  int16_t itemp;
  float diff, k;
  float ftemp1, ftemp2;
  int index;
  float outdoor_temp;

  outdoor_temp = get_outdoor_temp();
  
  if(outdoor_temp < -40.0){
    outdoor_temp = -40.0;
  }

  if(outdoor_temp > 15.0){
    outdoor_temp = 15.0;
  }

  itemp = ( int16_t )outdoor_temp;
  index = binary_search_int16t(outdoor_temp_seg, OUTDOOR_TEMP_SEG_MAX, itemp);

  if((index == 0) || (index == (OUTDOOR_TEMP_SEG_MAX - 1))){
    return Words2Float(&usRegHoldingBuf[MB_INDEX_N40_TEMP + index*2], 3412);
  }

  itemp = ( int16_t )(outdoor_temp*10);
  if(itemp == outdoor_temp_seg[index]*10){
    return Words2Float(&usRegHoldingBuf[MB_INDEX_N40_TEMP + index*2], 3412);
  }

  /* the outdoor temp is null int, but float ,ex. -10.6 */
  if((outdoor_temp < 0) && (index > 0)){
    // when the outdoor temp < 0， the index should - 1
    index -= 1;
  }

  ftemp1 = Words2Float(&usRegHoldingBuf[MB_INDEX_N40_TEMP + index*2], 3412);
  ftemp2 = Words2Float(&usRegHoldingBuf[MB_INDEX_N40_TEMP + (index + 1)*2], 3412);
  diff = fabs(ftemp1 - ftemp2);
  diff /= 10;

  k = (outdoor_temp - ( float )outdoor_temp_seg[index]);
  k *= 10;

  if(ftemp1 > ftemp2){
    return ftemp2 += k*diff;
  }else{
    return ftemp1 += k*diff;
  }
}


float get_qh_target_temp(u16 mode)
{
  float res = 50.0;
  switch(mode){
  case QH_MODE_MANUAL:
  case QH_MODE_SET:
    res = get_set_mode_target();
    break;
  case QH_MODE_CALC:
    res = get_cacl_mode_target();
    break;
  case QH_MODE_AI:
    res = get_AI_mode_target();
    break;
  }
  return res;
}

/*
 * get qihou control segment time offset
 */
float get_qh_segment_offset(void)
{
  int i;
  u16 time_pv;
  u16 seg_start[8], seg_end[8];
  u8 seg_index = 0;

  time_pv = usRegHoldingBuf[MB_INDEX_CLOCK_HOUR]*60 + usRegHoldingBuf[MB_INDEX_CLOCK_MIN];

  for(i = 0; i < 8; i++){
    seg_start[i] = usRegHoldingBuf[MB_INDEX_SEG1_START_HOUR + i*4]*60 + usRegHoldingBuf[MB_INDEX_SEG1_START_MIN + i*4];
    seg_end[i] = usRegHoldingBuf[MB_INDEX_SEG1_END_HOUR + i*4]*60 + usRegHoldingBuf[MB_INDEX_SEG1_END_MIN + i*4];
  }

  for(i = 0; i < 8; i++){
    if(seg_start[i] < seg_end[i]){
      if((time_pv >= seg_start[i]) && 
         (time_pv <= seg_end[i])){
           seg_index = i + 1;
           break;
      }
    }else if(seg_start[i] > seg_end[i]){  // accross 0 clock
      if(((time_pv >= seg_start[i]) && (time_pv <= 1439)) || 
         (time_pv == 0) || (time_pv < seg_end[i])){
           seg_index = i + 1;
           break;
      }
    }else{
      continue;
    }
  }

  if(seg_index > 0){  // find the seg index
      if(usRegHoldingBuf[MB_INDEX_CLOCK_WEEK] > 5){ // saturday and sunday offset disable
        if(usRegHoldingBuf[MB_INDEX_SEG1_SW + seg_index - 1] > 0){
          return 0.0;
        }
      }
      return Words2Float(&usRegHoldingBuf[MB_INDEX_SEG1_OFFSET + (seg_index - 1)*2], 3412);
  }else{
    return 0.0;
  }
}


float get_qh_holiday_offset(void)
{
  if(usRegHoldingBuf[MB_INDEX_HOLIDAY_STATE] > 0){
    return Words2Float(&usRegHoldingBuf[MB_INDEX_HOLIDAY_OFFSET], 3412);
  }
  return 0.0;
}

float get_qh_solar_offset(void)
{
  u16 time_pv, start_time, end_time;
  if(usRegHoldingBuf[MB_INDEX_SOLAR_SW] > 0){
    time_pv = usRegHoldingBuf[MB_INDEX_CLOCK_HOUR]*60 + usRegHoldingBuf[MB_INDEX_CLOCK_MIN];
    start_time = usRegHoldingBuf[MB_INDEX_SOLAR_START_HOUR]*60 + usRegHoldingBuf[MB_INDEX_SOLAR_START_MIN];
    end_time = usRegHoldingBuf[MB_INDEX_SOLAR_END_HOUR]*60 + usRegHoldingBuf[MB_INDEX_SOLAR_END_MIN];
    if((time_pv >= start_time) && (time_pv <= end_time)){
      return Words2Float(&usRegHoldingBuf[MB_INDEX_SOLAR_OFFSET], 3412);
    }
  }

  return 0.0;
}

/*
 * qihou target temp high and low limit compare
 */
float qh_target_temp_hl_cmp(float target)
{
  float hl, ll;
  ll = Words2Float(&usRegHoldingBuf[MB_INDEX_TARGET_TEMP_LL], 3412);
  hl = Words2Float(&usRegHoldingBuf[MB_INDEX_TARGET_TEMP_HL], 3412);
  if(target > hl){
    return hl;
  }

  if(target < ll){
    return ll;
  }

  return target;
}



/*---qihou control's feedback value update and get---*/
static float g_qh_feedback = 0.0;
static int update_qh_feedback(float data)
{
  g_qh_feedback = data;
  Float2Words(data, &usRegHoldingBuf[MB_INDEX_QH_FEEDBACK], 3412);
  return 0;
}

float get_qh_feedback(void)
{
  return g_qh_feedback;
}

/*---water control's feedback value update and get---*/
static float g_water_feedback = 0.0;
static int update_water_feedback(float data)
{
  g_water_feedback = data;
  Float2Words(data, &usRegHoldingBuf[MB_INDEX_WATER_LEVEL], 3412);
  return 0;
}

float get_water_feedback(void)
{
  return g_water_feedback;
}


/*---outdoor local value update and get---*/
float g_outdoor_temp_loc = 0.0;
static int update_outdoor_temp_loc(float data)
{
  g_outdoor_temp_loc = data;
  Float2Words(data, &usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_LOC], 3412);
  return 0;
}

float get_outdoor_temp_loc(void)
{
  return g_outdoor_temp_loc;
}


int update_qh_feedback_valueAndRegs(u8 current_adc_chn, float current_adc_value)
{
  if(current_adc_chn == usRegHoldingBuf[MB_INDEX_QH_FEEDBACK_CHN]){
    return update_qh_feedback(current_adc_value);
  }

  if(current_adc_chn == usRegHoldingBuf[MB_INDEX_WATER_LEVEL_CHN]){
    return update_water_feedback(current_adc_value);
  }

  if(current_adc_chn == usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_LOC_CHN]){
    return update_outdoor_temp_loc( current_adc_value );
  }
  return 0;
}



static u8 flag_add_water = 0;
static void water_level_control(void)
{
  float feedback, hl, ll;
  feedback = get_water_feedback();

  hl = Words2Float(&usRegHoldingBuf[MB_INDEX_WATER_LEVEL_HL], 3412);
  ll = Words2Float(&usRegHoldingBuf[MB_INDEX_WATER_LEVEL_LL], 3412);
  
  if(feedback > hl){
    flag_add_water = 0;
  }

  if(feedback < ll){
    flag_add_water = 1;
  }

  switch(usRegHoldingBuf[MB_INDEX_WATER_LEVEL_CHN]){
  case 2:
    if(flag_add_water > 0){
      usRegHoldingBuf[MB_INDEX_DO2] = 1;
    }else{
      usRegHoldingBuf[MB_INDEX_DO2] = 0;
    }
    break;
  case 3:
    if(flag_add_water > 0){
      usRegHoldingBuf[MB_INDEX_DO3] = 1;
    }else{
      usRegHoldingBuf[MB_INDEX_DO3] = 0;
    }
    break;
  case 4:
    if(flag_add_water > 0){
      usRegHoldingBuf[MB_INDEX_DO4] = 1;
    }else{
      usRegHoldingBuf[MB_INDEX_DO4] = 0;
    }
    break;
  case 5:
    if(flag_add_water > 0){
      usRegHoldingBuf[MB_INDEX_DO5] = 1;
    }else{
      usRegHoldingBuf[MB_INDEX_DO5] = 0;
    }
    break;
  default:
    if(flag_add_water > 0){
      usRegHoldingBuf[MB_INDEX_DO1] = 1;
    }else{
      usRegHoldingBuf[MB_INDEX_DO1] = 0;
    }
    break;
  }
}


/*
static u8 qh_mode_change_parse(void)
{
  static u16 last_mode = 0;
  if(usRegHoldingBuf[MB_INDEX_MODE] != last_mode){
    last_mode = usRegHoldingBuf[MB_INDEX_MODE];  
    return 1;
  }
  return 0;
}
*/


#define OUTPUT_INIT               30.0
void  qh_main_task(void *p_arg)
{
  float target, feedback, output, output_lower;
  float outdoor_temp;
  u32 adjust_cnt = 0;
  ai_control_calc_para_t *pqh_ai_ctrl_para;

  // set output init value 
  output = OUTPUT_INIT;
  //pqh_ai_ctrl_para = getAndUpdate_qh_ai_ctrlPara();
  while(1){
    // get target temp                                                       
    target = get_qh_target_temp( usRegHoldingBuf[MB_INDEX_MODE] );
    // get time seg offset, in set/calc/AI mode                   
    if(usRegHoldingBuf[MB_INDEX_MODE] > QH_MODE_MANUAL){
      target += get_qh_segment_offset();
    }
    // get solar/holiday offset in AI mode 
    if(usRegHoldingBuf[MB_INDEX_MODE] == QH_MODE_AI){
      target += get_qh_solar_offset();
      target += get_qh_holiday_offset();
    }
    // calc and AI mode target compare to high limit And low limit 
    if(usRegHoldingBuf[MB_INDEX_MODE] > QH_MODE_SET){
      qh_target_temp_hl_cmp(target);
    }
    // update target temp to regs 
    Float2Words(target, &usRegHoldingBuf[MB_INDEX_TARGET_TEMP], 3412);
    // get outdoor temp 
    outdoor_temp = get_outdoor_temp();
    // update outdoor temp to regs 
    Float2Words(outdoor_temp, &usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP], 3412);
    // get qh feedback 
    feedback = get_qh_feedback();
    // this app no need update qh feedback to regs 

    output_lower = Words2Float(&usRegHoldingBuf[MB_INDEX_OUTPUT_LL], 3412);

    // qh control 
    if(usRegHoldingBuf[MB_INDEX_MODE] != QH_MODE_MANUAL){
      if(get_running_secs_update_flag() > 0){
        adjust_cnt++;
        reset_running_secs_update_flag();
      }

      if(adjust_cnt >= usRegHoldingBuf[MB_INDEX_ADJUST_TIME]){
        adjust_cnt = 0;
        pqh_ai_ctrl_para = getAndUpdate_qh_ai_ctrlPara();
        output += ai_control_calc(pqh_ai_ctrl_para, target, feedback);
        // update output to manual output 
        Float2Words(output, &usRegHoldingBuf[MB_INDEX_MANUAL_OUTPUT], 3412);
      }
    }else{
      output = Words2Float(&usRegHoldingBuf[MB_INDEX_MANUAL_OUTPUT], 3412);
    }
    
    output = HL_LimitJudge(output, output_lower, 100.0);
    Float2Words(output, &usRegHoldingBuf[MB_INDEX_OUTPUT], 3412);

    hal_dac_output(usRegHoldingBuf[MB_INDEX_ACTUATOR_TYPE],
                  usRegHoldingBuf[MB_INDEX_AO_SIGNAL_TYPE], 
                  output);

    water_level_control();
                  
    OSTimeDlyHMSM(0, 0, 0, 10);
  }
}
