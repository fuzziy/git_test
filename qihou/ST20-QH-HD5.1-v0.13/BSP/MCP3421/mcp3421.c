#include "mcp3421.h"
#include "my_modbus.h"

/*
*******************************************************************************
*
*                               MCP3421 config
*
* 1. Data rate options
*   3.75 SPS(18bits)
*   15   SPS(16bits)
*   60   SPS(14bits)
*   140  SPS(12bits)
*
* 2. MCP3421 have only 1 config register
*   bit 7   RDY: Ready Bit
*             This bit is the data ready flag. In read mode, this bit indicates 
*             if the output register has been updated with a new conversion. 
*             In One-Shot Conversion mode, writing this bit to ��1�� initiates 
*             a new conversion.
*             
*             Reading RDY bit with the read command:
*             1 = Output register has not been updated.
*             0 = Output register has been updated with the latest conversion data.
*             
*             Writing RDY bit with the write command:
*             Continuous Conversion mode: No effect
*             One-Shot Conversion mode:
*             1 = Initiate a new conversion.
*             0 = No effect.
*   bit 6-5   C1-C0: Channel Selection Bits
*             These are the Channel Selection bits, but not used in the MCP3421 device.
*   bit 4 O/C: Conversion Mode Bit
*             1 = Continuous Conversion Mode. Once this bit is selected, the device 
*                 performs data conversion continuously.
*             0 = One-Shot Conversion Mode. The device performs a single conversion 
*                 and enters a low power standby mode until it receives another 
*                 write/read command.
*   bit 3-2 S1-S0: Sample Rate Selection Bit
*           00 = 240 SPS (12 bits),
*           01 = 60 SPS (14 bits),
*           10 = 15 SPS (16 bits),
*           11 = 3.75 SPS (18 bits)
*   bit 1-0 G1-G0: PGA Gain Selector Bits
*           00 = 1 V/V,
*           01 = 2 V/V,
*           10 = 4 V/V,
*           11 = 8 V/V
*
*******************************************************************************
*/


/*
*******************************************************************************
*
*                       Mcp3421_ch1_SendByte(u8 byte)
*
* ���������һ��MCP3421����1���ֽڣ���ʵ��������MCP3421
* 
* ������byte       Ҫд���ֽ�����
*
* �������裺�ȷ����豸��ַ+д��־λ��Ȼ�������ֽ�Ĭ�Ͼ���д�����üĴ���
*
*******************************************************************************
*/
void Mcp3421_Ch1_SendByte(u8 byte)
{
    ADC1_IIC_Start();
    ADC1_IIC_SendByte(MCP3421_W);            //дģʽ
    ADC1_IIC_WaitAck();
    ADC1_IIC_SendByte(byte);
    ADC1_IIC_WaitAck();
    ADC1_IIC_Stop();
}

/*
*******************************************************************************
*
*                       Mcp3421_ch1_ReadData(void)
*
* ���������һ��MCP3421��ȡ����
* 
* ������null
*
* �������裺�ȷ����豸��ַ+����־λ��Ȼ��������ȡ2���ֽڼ���
*
* ��ע������Ŀ�в���14λ+2�����棬����ֻ��Ҫ��ȡǰ�����ֽڼ��ɡ�
*       ��Ч����λΪD13-D0
*******************************************************************************
*/
u16  Mcp3421_Ch1_ReadData(void)
{
    u16 voltage = 0;
    u8 step,temp=0;
    ADC1_IIC_Start();
    ADC1_IIC_SendByte(MCP3421_R);            //��ģʽ
    ADC1_IIC_WaitAck();
    //��ʼ��������
    for(step = 0; step < 2; step++)     //ֻҪǰ2���ֽ�
    {
      if(step < 1)
        temp = ADC1_IIC_ReadByte(1);      //�ظ�ack
      else
        temp = ADC1_IIC_ReadByte(0);      //�ظ�nack
      voltage = (voltage<<8) | temp;
    }
    
    ADC1_IIC_Stop();
    voltage &= 0x1FFF;                  //��Ч����λΪD13-D0
    return voltage;
}


/*
 * MCP3421 16λ�ֱ��ʣ�����1ģʽ�½��ж�ȡ����
 *
 * ����ֵ�� MCP3421 �����ĵ�ѹֵ
 */
static float mcp3421_14bit_vol_read(void)
{
    u16 idata;
    float fdata = 0.0;
    
    Mcp3421_Ch1_SendByte(MCP3421_GA1_14B);
    idata = Mcp3421_Ch1_ReadData();

    fdata = ( float )idata;
    fdata = (fdata/MCP3421_14B_URANGE)*2.048;  

    if(fdata < 0){
      return 0.0;
    }

    if(fdata > 2.048){
      return 2.048;
    }

    return fdata;
}



static float mcp3421_16bit_vol_read(void)
{
  float res = 0;
  u16 data = 0;
  u8 tmp=0;
    Mcp3421_Ch1_SendByte(MCP3421_GA1_16B);
    ADC1_IIC_Start();
    ADC1_IIC_SendByte(MCP3421_R);
    ADC1_IIC_WaitAck();
    tmp = ADC1_IIC_ReadByte(1); //
    data |= tmp;
    tmp = ADC1_IIC_ReadByte(0); 
    data = (data<<8)|tmp;
    ADC1_IIC_Stop();
    data &= 0x7FFF; //
    res = ( float )data;
    res = res*2.048/MCP3421_16B_URANGE;
    return res;
}

float mcp3421_vol_read(u8 sampling_bits)
{
  if(sampling_bits == 16){
    return mcp3421_16bit_vol_read();
  }else{
    return mcp3421_14bit_vol_read();
  }
}


/*
*******************************************************************************
*
*                       Mcp3421_ch1_SendByte(u8 byte)
*
* ���������һ��MCP3421����1���ֽڣ���ʵ��������MCP3421
* 
* ������byte       Ҫд���ֽ�����
*
* �������裺�ȷ����豸��ַ+д��־λ��Ȼ�������ֽ�Ĭ�Ͼ���д�����üĴ���
*
*******************************************************************************
*/
void Mcp3421_Ch2_SendByte(u8 byte)
{
    ADC2_IIC_Start();
    ADC2_IIC_SendByte(MCP3421_W);            //дģʽ
    ADC2_IIC_WaitAck();
    ADC2_IIC_SendByte(byte);
    ADC2_IIC_WaitAck();
    ADC2_IIC_Stop();
}

/*
*******************************************************************************
*
*                       Mcp3421_ch1_ReadData(void)
*
* ���������һ��MCP3421��ȡ����
* 
* ������null
*
* �������裺�ȷ����豸��ַ+����־λ��Ȼ��������ȡ2���ֽڼ���
*
* ��ע������Ŀ�в���14λ+2�����棬����ֻ��Ҫ��ȡǰ�����ֽڼ��ɡ�
*       ��Ч����λΪD13-D0
*******************************************************************************
*/
u16  Mcp3421_Ch2_ReadData(void)
{
    u16 voltage = 0;
    u8 step,temp=0;
    ADC2_IIC_Start();
    ADC2_IIC_SendByte(MCP3421_R);            //��ģʽ
    ADC2_IIC_WaitAck();
    //��ʼ��������
    for(step = 0; step < 2; step++)     //ֻҪǰ2���ֽ�
    {
      if(step < 1)
        temp = ADC2_IIC_ReadByte(1);      //�ظ�ack
      else
        temp = ADC2_IIC_ReadByte(0);      //�ظ�nack
      voltage = (voltage<<8) | temp;
    }
    
    ADC2_IIC_Stop();
    voltage &= 0x3FFF;                  //��Ч����λΪD13-D0
    return voltage;
}


