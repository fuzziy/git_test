#ifndef __CALCULATE_H
#define __CALCULATE_H
#include "stm32f10x.h"

#define max(a,b)     (a>b? a:b)
#define min(a,b)     (a<b? a:b)
#define range(x,a,b) (min(max(x,a),b))


u8 HexToDec(u8 data);
u16 CRC16(u8 *puchMsg,u16 usDataLen);
u16 LRC(u8 *rtu_data,u16 rtu_len);
void DEC2BCD(u16 dec,u8 *bcd,u8 len);
u8 BCD2HEX(u8 data);
u8 BCD2DEC(u8 bcd);
u8 HEX2BCD(u8 data);
u8 Hex2Ascii(u8 nHex);
u16 RTU2ASCII(u8 data);
u8 Ascii2Hex(u8 nAscii);
u8 TwoAscii2Hex(u8 *ascii_data);
void FloatToByte(float para,u8* data);
u8 Float2Ascii(float para,u8* data);
float BytesToFloat(u8* data,u16 mode);
float Words2Float(u16* data,u16 mode);
u16 SwapU16(u16 data);
void Float2Bytes(float para,u8* data,u16 mod);
void Float2Words(float para,u16* data,u16 mod);
u8 CS_Calc(u8 *data,u16 len);
u16 BinarySearchInterval(float *array,u16 len,float T);
u8 FloatEqual(float f1,float f2);
u8	MBCHAR2BIN( u8 ucCharacter );
u8	MBBIN2CHAR( u8 ucByte );
u8	MBLRC( u8 * pucFrame, u16 usLen );
void Bytes2Words(u16 *des, u8 *src);
u16  TwoByte2Word(u8 *byte);
void Words2Bytes(u16 *words, u8 *bytes, u16 len);

float PressCalc( u8 type, float data, float range );
float TemperatureCalc( float data, float range_min ,float range_max );
float WaterLevelCalc( float data, float range );
u16 ValveCalc( float data, float range );


float HL_LimitJudge(float data, float low_limit, float high_limit);
float Output2mA(float data, u16 type);
float variable_correction_calc(float original_value, float zero_k, float full_k);
u8 FloatIntervalJudge(float data, float seg1, float seg2);
u8 Para_IncJudge(float data1, float data2, float data3, float data4);
int binary_search_int16t(int16_t *num, int num_cnt, int16_t target);


#endif

