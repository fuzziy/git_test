#define PARA_GLOBALS
#include "qihou_para.h"
#include "my_modbus.h"
#include "24cxx.h"
#include "calculate.h"
#include "bsp.h"
#include <string.h>
#include "iwdg.h"


#define EEPROM_READ_REPEAT_MAX      3
#define EEPROM_ONE_TIME_WRITE_MAX   100
void qh_para_init(void)
{
  u8 buf[1024] = {0};
  int i;
  u8 repeat_cnt = 0;
  u8 flag_read_ok = 0;
  int nleft, write_byte_num, write_cnt;
  memset(buf, 0, 1024);

  while(repeat_cnt++ < 3){
    q_AT24CXX_Read(0, buf, EE_PARA_NBYTES);
    if((buf[0] == EE_FLAG_CS_H) &&
       (buf[1] == EE_FLAG_CS_L) &&
       (buf[EE_PARA_NBYTES - 2] == EE_FLAG_CRC_H) &&
       (buf[EE_PARA_NBYTES - 1] == EE_FLAG_CRC_L)){
           flag_read_ok = 1;
           break;
       }
  }

  if( flag_read_ok ){
      for(i = 0; i < SAVE_REGS_NUM; i++){
          usRegHoldingBuf[MB_INDEX_START_SAVE + i] = TwoByte2Word(&buf[2 + i*2]);
      }
  }else{    //reset
    usRegHoldingBuf[MB_INDEX_ERR_CODE] = ERR_EEPROM_READ;
    buf[0] = EE_FLAG_CS_H;
    buf[1] = EE_FLAG_CS_L;
    
    Float2Words(46.0, &usRegHoldingBuf[MB_INDEX_SET_TEMP], 3412);
    Float2Words(48.0, &usRegHoldingBuf[MB_INDEX_START_TEMP], 3412);
    Float2Words(5, &usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_REM], 3412);
    Float2Words(5, &usRegHoldingBuf[MB_INDEX_FEEL_TEMP], 3412);
    Float2Words(0.0, &usRegHoldingBuf[MB_INDEX_FEEL_TEMP_K], 3412);
    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_CURVATURE1], 3412);
    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_CURVATURE2], 3412);
    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_CURVATURE3], 3412);
    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_CURVATURE4], 3412);
    Float2Words(60.0, &usRegHoldingBuf[MB_INDEX_MANUAL_OUTPUT], 3412);
    Float2Words(0.0, &usRegHoldingBuf[MB_INDEX_HOLIDAY_OFFSET], 3412);
    Float2Words(3, &usRegHoldingBuf[MB_INDEX_WATER_LEVEL_HL], 3412);
    Float2Words(0.6, &usRegHoldingBuf[MB_INDEX_WATER_LEVEL_LL], 3412);
    Float2Words(20.0, &usRegHoldingBuf[MB_INDEX_OUTPUT_LL], 3412);
    Float2Words(60.0, &usRegHoldingBuf[MB_INDEX_TARGET_TEMP_HL], 3412);
    Float2Words(30.0, &usRegHoldingBuf[MB_INDEX_TARGET_TEMP_LL], 3412);
    Float2Words(0.0, &usRegHoldingBuf[MB_INDEX_SOLAR_OFFSET], 3412);

    usRegHoldingBuf[MB_INDEX_MODE] = 0;
    usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_SW] = 0;
    usRegHoldingBuf[MB_INDEX_QH_FEEDBACK_CHN] = 2;
    usRegHoldingBuf[MB_INDEX_OUTDOOR_TEMP_LOC_CHN] = 1;
    usRegHoldingBuf[MB_INDEX_WATER_LEVEL_CHN] = 6;
    usRegHoldingBuf[MB_INDEX_WATER_CTRL_CHN] = 1;

    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_ERR_SEG1], 3412);
    Float2Words(1.0, &usRegHoldingBuf[MB_INDEX_ERR_SEG2], 3412);
    Float2Words(1.5, &usRegHoldingBuf[MB_INDEX_ERR_SEG3], 3412);
    Float2Words(2.0, &usRegHoldingBuf[MB_INDEX_ERR_SEG4], 3412);

    Float2Words(0.5, &usRegHoldingBuf[MB_INDEX_ERR_SEG1_OUTPUT], 3412);
    Float2Words(1.0, &usRegHoldingBuf[MB_INDEX_ERR_SEG2_OUTPUT], 3412);
    Float2Words(2.0, &usRegHoldingBuf[MB_INDEX_ERR_SEG3_OUTPUT], 3412);
    Float2Words(3.0, &usRegHoldingBuf[MB_INDEX_ERR_SEG4_OUTPUT], 3412);
    Float2Words(0.2, &usRegHoldingBuf[MB_INDEX_ERR_CHANGE], 3412);
    usRegHoldingBuf[MB_INDEX_ADJUST_TIME] = 30;

    for(i = 0; i < 56; i++){
        Float2Words(50.0, &usRegHoldingBuf[MB_INDEX_N40_TEMP + i*2], 3412);
    }

    usRegHoldingBuf[MB_INDEX_COM1_BAUD] = 9600;
    usRegHoldingBuf[MB_INDEX_COM1_DATABITS] = 8;
    usRegHoldingBuf[MB_INDEX_COM1_PARITY] = 0;
    usRegHoldingBuf[MB_INDEX_COM2_BAUD] = 9600;
    usRegHoldingBuf[MB_INDEX_COM2_DATABITS] = 8;
    usRegHoldingBuf[MB_INDEX_COM2_PARITY] = 0;

    for(i = 0; i < SAVE_REGS_NUM; i++){
        buf[2 + i*2] = ( u8 )(usRegHoldingBuf[MB_INDEX_START_SAVE + i] >> 8);
        buf[2 + i*2 + 1] = ( u8 )(usRegHoldingBuf[MB_INDEX_START_SAVE + i]&0xFF);
    }

    buf[EE_PARA_NBYTES - 2] = EE_FLAG_CRC_H;
    buf[EE_PARA_NBYTES - 1] = EE_FLAG_CRC_L;

    nleft = EE_PARA_NBYTES;
    write_cnt = 0;
    while(nleft > 0){
        if(nleft > EEPROM_ONE_TIME_WRITE_MAX){
            write_byte_num = EEPROM_ONE_TIME_WRITE_MAX;
        }else{
            write_byte_num = nleft;
        }

        q_AT24CXX_Write(write_cnt , buf + write_cnt, write_byte_num);
        
        sp706s_feed();

        write_cnt += write_byte_num;
        nleft -= write_byte_num;
    }
  }
}