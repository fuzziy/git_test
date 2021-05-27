#include "adc.h"
#include "mcp3421.h"
#include "74hc405x.h"
#include "my_modbus.h"
#include "calculate.h"
#include "qihou.h"
#include "wrappers.h"



static float adc_input_mA_offset(float mA)
{
  if(mA < 10.0){
    return 0.01;
  }else{
    return 0.02;
  }
}
/*
 * AI输入电流值计算
 * 
 * 输入电流信号，采样电阻为150Ω，则
 *      0~20mA  ---  0~3V
 *   然后，经过15K/10K (0.4)进行分压缩小
 *      0~3V -- 0~1.2V
 * 
 * 反向计算时，先除0.4，然后再除0.15
 * 
 * @para[in]  mcp3421_in_vol -- mcp3421的输入电压值
 * 
 * @return:   AI通道的输入电流值，单位mA
 * 
 */ 
static float analog_input_mA_calc(float mcp3421_in_vol)
{
  float res, offset;
  if(mcp3421_in_vol < 0){
    return 0.0;
  }

  res = mcp3421_in_vol/0.4;
  res /= 0.15;
  
  offset = adc_input_mA_offset( res );
  res = res - offset;
  return res;
}



static float adc_input_voltage_offset(float vol)
{
  if(vol < 4.5){
    return 0.01;
  }
  else if(vol < 6.6){
    return 0.014;
  }
  else if(vol < 8){
    return 0.018;
  }else{
    return 0.021;
  }
}
/*
 *  AI输入电压值计算
 * 
 * .输入电压信号，0~10V，分压电阻200/200
 *      0~10V -- 0~5V
 *   然后，经过15K/10K (0.4)进行分压缩小
 *      0~5V -- 0~2V
 * 
 * 反向计算时，先除0.4，然后再除0.5
 * 
 * @para[in]  mcp3421_in_vol -- mcp3421的输入电压值
 * 
 * @return:   AI通道的输入电压值，单位V
 * 
 */ 
static float analog_input_voltage_calc(float mcp3421_in_vol)
{
    float res, offset;
    if(mcp3421_in_vol < 0){
      return 0.0;
    }

    res = mcp3421_in_vol/0.4;
    res /= 0.5;

    offset = adc_input_voltage_offset( res );
    res = res - offset;
    return res;
}




/*
 * AI输入的远传压力表电阻值
 * 
 * 3.输入为远传压力表接5V分压信号(10~380Ω举例)，不采样直接输入
 *      0~380Ω -- 0  ~ 5V
 *   然后，经过15K/10K (0.4)进行分压缩小
 *      0~5V -- 0~2V
 * 
 * 反向计算时，先除0.4，然后再除 5/PRESS_SENSOR_OHM_MAX
 * 
 * @para[in]  mcp3421_in_vol -- mcp3421的输入电压值
 * 
 * @return:   AI通道的输入的电阻值，单位欧姆
 */ 
#define PRESS_SENSOR_OHM_MIN    10
#define PRESS_SENSOR_OHM_MAX    380
static float analog_input_ohm_calc(float mcp3421_in_vol)
{
    float res;
    if(mcp3421_in_vol < 0){
      return 0.0;
    }

    res = mcp3421_in_vol/0.4;
    res = res*PRESS_SENSOR_OHM_MAX/5;
    return res;
}

/*
 * 计算AI输入信号的原始值，获取对应的mA/V/ohm
 * 
 * @para[in]  mcp3421_in_vol    MCP3421的输入电压值
 * @para[in]  input_typ         输入信号类型
 * 
 * @return: 返回AI输入的信号值，电流、电压、欧姆
 * 
 */
static float get_analog_input_siginal_value(float mcp3421_in_vol, u16 input_type)
{
  switch( input_type ){
  case AI_INPUT_MA:     // 0 -- AI input type is 4~20mA
    return analog_input_mA_calc( mcp3421_in_vol );
    break;
  case AI_INPUT_NORMAL_VOL:
    return analog_input_voltage_calc( mcp3421_in_vol );
    break;
  case AI_INPUT_OHM2VOL:
    return analog_input_ohm_calc( mcp3421_in_vol );
    break;
  default:
    return 0.0;
    break;
  }
}


/*
 *  mA2varcalc: 4~20mA convert to variable calculate
 *              if range_l == range_h , return mA value
 *  
 * 
 * @para[in]  mA      --  current mA value
 * @para[in]  range_l --  variable range low limit
 * @para[in]  range_h --  variable range high limit
 *
 * @return  if range_l == range_h, return mA
 *          else   return variable
 */
static float mA_convert_var_calc(float mA, float range_l, float range_h)
{
    float k, b;
    if(FloatEqual(range_l, range_h)){
      return mA;
    }
    
    k = (range_h - range_l)/16.0;
    b = range_l - 4*k;
    return (mA*k + b);
}

/*
 *  des: 0-10V convert to variable calculate
 *              if range_l == range_h , return voltage value
 *  
 * 
 * @para[in]  voltage      --  current voltage value
 * @para[in]  range_l --  variable range low limit
 * @para[in]  range_h --  variable range high limit
 *
 * @return  if range_l == range_h, return voltage
 *          else   return variable
 */
static float voltage_convert_var_calc(float voltage, float range_l, float range_h)
{
    float res = 0.0;
    float k, b;
    if(FloatEqual(range_l, range_h)){
      return voltage;
    }
    // 0~10 V  -- range_l ~ range_h
    k = (range_h - range_l)/10;
    b = range_l;

    res = voltage*k + b;
   
    return res;
}


/*
 *   10-380ohm convert to press calculate
 *              if range_l == range_h , return ohm value
 *  
 * 
 * @para[in]  ohm      --  current ohm value
 * @para[in]  range_l --  variable range low limit
 * @para[in]  range_h --  variable range high limit
 *
 * @return  if range_l == range_h, return ohm
 *          else   return variable
 */
static float ohm_convert_var_calc(float ohm, float range_l, float range_h)
{
    float res = 0.0;
    float k, b;
    if(FloatEqual(range_l, range_h)){
      return ohm;
    }
    // 10 ~ 380Ω  -- range_l ~ range_h
    k = (range_h - range_l)/(PRESS_SENSOR_OHM_MAX - PRESS_SENSOR_OHM_MIN);
    b = range_l - PRESS_SENSOR_OHM_MIN*k;
    res = k*ohm + b;

    return res;
}


/*
 * analog input valiable calc, mA/vol/ohm convert to variable
 * 
 * @para[in]   original_vale  -- original value (mA/vol/ohm)
 * @para[in]   input_type     -- mA/vol/ohm
 * @para[in]   range_l        -- variable min
 * @para[in]   range_h        -- variable max
 * 
 * @return   the variable of AI
 * 
 */
static float analog_input_variable_calc(float original_value, u16 input_type, float range_l, float range_h)
{



  switch( input_type ){
  case AI_INPUT_MA:     // 0 -- AI input type is 4~20mA
    return mA_convert_var_calc(original_value, range_l, range_h);
    break;
  case AI_INPUT_NORMAL_VOL:
    return voltage_convert_var_calc(original_value, range_l, range_h);
    break;
  case AI_INPUT_OHM2VOL:
    return ohm_convert_var_calc(original_value, range_l, range_h);
    break;
  default:
    return 0.0;
    break;
  }
}


/*
 * analog input variable filter calc
 * 
 * @para[in]   ai_chn   -- 1~8
 * @para[in]   value    -- variable value
 * @para[in]   filter_k -- filter k
 * 
 * @return    the result of after filter.
 * 
 */
static float analog_input_variable_last[10] = {0};
static float analog_input_variable_filter_calc(u8 ai_chn, float value, float filter_k)
{
  float res = 0;
  if(filter_k > 0.99){
    filter_k = 0.99;
  }

  if(filter_k < 0){
    return value;
  }

  res = value*(1 - filter_k) + filter_k*analog_input_variable_last[ai_chn];
  analog_input_variable_last[ai_chn] = res;
  return res;
}


/*
 * get the AI variable result and update AI register
 * the result is the end value, after zero_k, full_k, filter_k calc
 * 
 * @para[in]  ai_chn  -- AI chn 1~8
 * 
 * @return   the end result of AI variable.
 * 
 */
float get_AI_variable_result(u8 ai_chn)
{
    float res = 0.0, mcp3421_in_vol = 0.0;
    float range_l, range_h;
    float zero_k, full_k;
    float filter_k;
    int offset, input_type;


    if(ai_chn == 0){
      ai_chn = 1;
    }

    if(ai_chn > 8){
      ai_chn = 8;
    }

    offset = ai_chn - 1;
    input_type = usRegHoldingBuf[MB_INDEX_AI1_TYPE + offset];
    //calculate k
    range_l = Words2Float(&usRegHoldingBuf[MB_INDEX_AI1_RANGE_L + offset*4], 3412);
    range_h = Words2Float(&usRegHoldingBuf[MB_INDEX_AI1_RANGE_H + offset*4], 3412);
    zero_k = Words2Float(&usRegHoldingBuf[MB_INDEX_AI1_ZERO_FAC + offset*2], 3412);
    full_k = Words2Float(&usRegHoldingBuf[MB_INDEX_AI1_FULL_FAC + offset*2], 3412);
    filter_k = Words2Float(&usRegHoldingBuf[MB_INDEX_AI1_FILTER_FAC + offset*2], 3412);
    //1. get mcp3421 input voltage
    //mcp3421_in_vol = mcp3421_14bit_vol_read();
    mcp3421_in_vol = mcp3421_vol_read(16);
    //2. use mcp3421 in voltage cal AI original value
    res = get_analog_input_siginal_value(mcp3421_in_vol, input_type);

    //3. analog input variable calc
    res = analog_input_variable_calc(res, input_type, range_l, range_h);

    //4. variable correction
    res = variable_correction_calc(res, zero_k, full_k);

    //5. variable filter calc
    res = analog_input_variable_filter_calc(ai_chn, res, filter_k);
    //6. range low/high compare
    if(range_l < range_h){
      if(res > range_h){
        res = range_h;
      }else if(res < range_l){
        res = range_l;
      }
    }
    //7. update AI register
    Float2Words(res, &usRegHoldingBuf[MB_INDEX_AI1 + offset*2], 3412);
    //8. update proj feedback value
    wrapper_update_proj_dataAndRegs(ai_chn, res);
    //9. return
    return res; 
}





