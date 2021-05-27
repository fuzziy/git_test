#ifndef __MCP3421_H
#define __MCP3421_H
#include <stm32f10x.h>
#include "sim_i2c.h"


/*
 * 0x94 = 1001 0100
 *
 * bit7         1 = ��ʼһ���µ�ת��
 *
 * bit6-5       ��������
 *
 * bit4         1 = ����ת��ģʽ(Ĭ�ϣ�
 *
 * bit3-2       S1-S0:������ѡ��λ
 *              00 = 240sps(12bit) Ĭ��   
 *              01 = 60sps (14bit) 
 *              10 = 15sps (16bit)
 *              11 = 3.75sps (18bit) 
 *
 * bit1-0       G1-G0:����ѡ��λ
 *              00 = x1  Ĭ��   
 *              01 = x2  
 *              10 = x4 
 *              11 = x4 
 *
 */

#define MCP3421_GA1_14B     0x94   // 1001 0100 
#define MCP3421_GA1_16B     0x98   // 1001 1000  

#define MCP3421_14B_URANGE  8191   // �ֱ�����14ʱ����Ӧ������޷������ֵΪ8192
#define MCP3421_16B_URANGE  32767  // �ֱ���Ϊ16bit����Ӧ����޷������ֵ 32768
#define MCP3421_W           0xD0   //д����
#define MCP3421_R           0xD1   //������


void Mcp3421_Ch1_SendByte(u8 byte);
u16  Mcp3421_Ch1_ReadData(void);

void Mcp3421_Ch2_SendByte(u8 byte);
u16  Mcp3421_Ch2_ReadData(void);

float mcp3421_14bit_vol_read(void);
float mcp3421_16bit_vol_read(void);
float mcp3421_vol_read(u8 sampling_bits);


#endif