/*
*********************************************************************************************************
*                                          ģ��I2Cͷ�ļ�
*
*                
*
* Filename      : simulated_i2c.h
* Version       : v1.00
* Programmer(s) : JSQ
*********************************************************************************************************
*/

#ifndef __SIM_I2C_H
#define __SIM_I2C_H
#include <stm32f10x.h>
#include "sys.h"

//IO�������ã�ͨ���Ĵ���ֱ���趨
//GPIOX_CRL�Ƕ�GPIOX�ĵ�8���˿ڽ������ã���PB0~PB7,GPIOX_CRH���ǶԸ�8���˿ڽ���
//                                                ���ã���PB8~PB16
/*
******************************************
***ADC1ʹ�õ�IIC�����Ŷ�Ӧ��ϵΪ
*  SCL--PE6
*  SDA--PE5
******************************************
*/
#define ADC1_SDA_IN()          {GPIOE->CRL &= 0xFF0FFFFF;GPIOE->CRL |= 0x00800000;}
#define ADC1_SDA_OUT()         {GPIOE->CRL &= 0xFF0FFFFF;GPIOE->CRL |= 0x00300000;}


/*
******************************************
***AT24C16�õ�IIC�����Ŷ�Ӧ��ϵΪ
*  SCL--PE0
*  SDA--PE1
******************************************
*/
#define EEPROM_SDA_IN()          {GPIOE->CRL &= 0xFFFFFF0F;GPIOE->CRL |= 0x00000080;}
#define EEPROM_SDA_OUT()         {GPIOE->CRL &= 0xFFFFFF0F;GPIOE->CRL |= 0x00000030;}



//IO��������
#define ADC1_IIC_SCL            PEout(6)
#define ADC1_IIC_SDA            PEout(5)
#define ADC1_READ_SDA           PEin(5)  //����SDA 



#define EEPROM_IIC_SCL            PEout(0)
#define EEPROM_IIC_SDA            PEout(1)
#define EEPROM_READ_SDA           PEin(1)  //����SDA 



// SCL -- PB5
// SDA -- PB8
#define RX8025_SDA_IN()  {GPIOB->CRH &= 0xFFFFFFF0;GPIOB->CRH |= 0x00000008;}
#define RX8025_SDA_OUT() {GPIOB->CRH &= 0xFFFFFFF0;GPIOB->CRH |= 0x00000003;}

#define RX8025_IIC_SCL            PBout(5)
#define RX8025_IIC_SDA            PBout(8)
#define RX8025_READ_SDA           PBin(8)  //





//IIC�����в�����
void IIC_Init(void);
//ADC1
void ADC1_IIC_Start(void);
void ADC1_IIC_Stop(void);
void ADC1_IIC_SendByte(u8 byte);
u8 ADC1_IIC_ReadByte(u8 ack);
u8 ADC1_IIC_WaitAck(void);
void ADC1_IIC_Ack(void);
void ADC1_IIC_NAck(void);
//ADC2
void ADC2_IIC_Start(void);
void ADC2_IIC_Stop(void);
void ADC2_IIC_SendByte(u8 byte);
u8 ADC2_IIC_ReadByte(u8 ack);
u8 ADC2_IIC_WaitAck(void);
void ADC2_IIC_Ack(void);
void ADC2_IIC_NAck(void);
//EEPROM
void EEPROM_IIC_Start(void);
void EEPROM_IIC_Stop(void);
void EEPROM_IIC_SendByte(u8 byte);
u8 EEPROM_IIC_ReadByte(u8 ack);
u8 EEPROM_IIC_WaitAck(void);
void EEPROM_IIC_Ack(void);
void EEPROM_IIC_NAck(void);



void RX8025_IIC_Start(void);
void RX8025_IIC_Stop(void);
void RX8025_IIC_SendByte(u8 byte);
u8 RX8025_IIC_ReadByte(u8 ack);
u8 RX8025_IIC_WaitAck(void);
void RX8025_IIC_Ack(void);
void RX8025_IIC_NAck(void);



#endif