#include "calculate.h"
#include "string.h"
#include "math.h"


///crc У������ѯ����
static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;


/* Table of CRC values for low-order byte */

static char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;
///////////////////////////////////////////////////



///////ʮ������ֱ��ת����ʮ���ƣ�����0X12��ת�����Ϊ12
u8 HexToDec(u8 data)
{
	u8 res;
	res = ((data&0xF0)>>4)*10+(data&0x0F);
	return res;
}

//CRC16  ����
u16 CRC16(u8 *puchMsg,u16 usDataLen)
{
 unsigned char uchCRCHi = 0xFF ; 
 unsigned char uchCRCLo = 0xFF ; 
 unsigned uIndex ; 
 while (usDataLen--) 
   {
    uIndex = uchCRCHi ^ *puchMsg++ ; 
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
    uchCRCLo = auchCRCLo[uIndex] ;
   }
 return (uchCRCHi << 8 | uchCRCLo) ;
}


/*
*************************************************************
*				MBCHAR2BIN( u8 ucCharacter )
*
* ���� : ����Modbus ASCII�н����յ���ASCII��ת����RTU�İ���
*				 ���磬���յ�����0x30����ת���� 0x00�����յ�����
*				 0x31,��ת���� 0x01
*
* ���� ��Ҫת����ASCII��
*
*	����ֵ��ת����Ľ��
*
*************************************************************
*/

u8	MBCHAR2BIN( u8 ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( u8 )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( u8 )( ucCharacter - 'A' + 0x0A );
    }
    else
    {
        return 0xFF;
    }
}

/*
*************************************************************
*						MBLRC( u8 * pucFrame, u16 usLen )
*
* ���� : ����Modbus ASCII�е�LRC����
*
* ���� ��Ҫ�����ָ�룬������
*
*	����ֵ��������
*
*************************************************************
*/

u8	MBLRC( u8 * pucFrame, u16 usLen )
{
    u8           ucLRC = 0;  /* LRC char initialized */

    while( usLen-- )
    {
        ucLRC += *pucFrame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( u8 ) ( -( ( u8 ) ucLRC ) );
    return ucLRC;
}


/*
*************************************************************
*						MBBIN2CHAR( u8 ucByte )
*
* ���� : ����Modbus ASCII�еĽ�RTUת��ΪASCII��
*				 ���磬�� 1 ת���� 0x31
*
* ���� ��Ҫ�������ֵ
*
*	����ֵ��������
*
*************************************************************
*/

u8
MBBIN2CHAR( u8 ucByte )
{
    if( ucByte <= 0x09 )
    {
        return ( u8 )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) )
    {
        return ( u8 )( ucByte - 0x0A + 'A' );
    }
    
    return '0';
}



//BCD��ת����ʮ���� ��
u8 BCD2HEX(u8 data)
{
	u8 temp;   
    temp=(data>>4)*10 + (data&0x0f);
    return temp;
}

//ʮ����ת����BCD��
u8 HEX2BCD(u8 data)
{
	u8 temp; 
    temp=((data/10)<<4) + (data%10);
    return temp; 	
}

//������ת��Ϊ4�ֽڱ�ʾ,��������˳�����Ҫ����4321��Ҳ��4321��������Ҫ���䵹��
void FloatToByte(float para,u8* data)
{
	int i;
	u8* p = (u8*)&para;
	for(i=0;i<sizeof(float);i++)
	{
		*data = *p;
		p++;
		data++;
	}
}


//������ת��Ϊ4���ֽ�
//mod��4�ֽ�˳��ģʽ
//     4321
//     3412
void Float2Bytes(float para,u8* data,u16 mod)
{
	u8 temp[4];
	switch(mod)
	{
		case 4321:
			FloatToByte(para,data);
			break;
		case 3412:
			FloatToByte(para,temp);
		  data[0] = temp[1];
			data[1] = temp[0];
			data[2] = temp[3];
			data[3] = temp[2];
			break;
	}
}

//������ת��Ϊ2����
//mod��4�ֽ�˳��ģʽ
//     4321
//     3412
void Float2Words(float para,u16* data,u16 mod)
{
	u8 buf[4];
	Float2Bytes(para,buf,mod);
	data[0] = (buf[0]<<8)|buf[1];
	data[1] = (buf[2]<<8)|buf[3];
}
	

//��һ��16λ���ĸߵ�8λ�ߵ�����0x4321����ߵ���Ϊ0x3412
u16 SwapU16(u16 data)
{
	u16 res;
	u8 temp_H,temp_L;
	temp_H = (data&0xFF00)>>8;
	temp_L = data&0xFF;
	res = (temp_L<<8)|temp_H;
	return res;
}

//��1��������ת��Ϊ8��ASCII��ʽ���ֽ�
u8 Float2Ascii(float para,u8* data)
{
	int i;
	u8 rtu_temp[4];
	u16 temp;
	Float2Bytes(para,rtu_temp,2143);
	for(i=0;i<4;i++)
	{
		temp = RTU2ASCII(rtu_temp[i]);
		data[i*2] = (temp&0xFF00)>>8;
		data[i*2+1] = temp&0xFF;	
	}
	return 8;
}


//������������16������ת��ΪASCII�ַ�
//���أ�Ascii�ַ�
u8 Hex2Ascii(u8 nHex)
{
	u8 res;
	if(nHex<=9)     //u8��ʽ����һ���Ǵ��ڵ���0
		res = nHex+0x30;
	else if((nHex>=0x0A)&&(nHex<=0x0F))   //A-F��д��ĸ
		res = nHex+0x37;
	else res = 0xFF;
	return res;
}

//������������ASCII�ַ�ת��Ϊ16����
//���أ�ʮ�������ַ�
u8 Ascii2Hex(u8 nAscii)
{
	u8 res;
	if((nAscii>=0x30)&&(nAscii<=0x39))    //����0-9
		res = nAscii-0x30;
	else if((nAscii>=0x41)&&(nAscii<=0x46))   //��д��ĸA-F
		res = nAscii-0x37;
	return res;
}

//������������2��ASCII�ַ�ת��Ϊ1��ʮ�������ַ�����0x30-0x031ת����0x01
//���أ�ʮ�������ַ�
u8 TwoAscii2Hex(u8 *ascii_data)
{
	u8 res,temp;
	temp = Ascii2Hex(ascii_data[0]);
	res = (temp<<4)|Ascii2Hex(ascii_data[1]);
	return res;
}


union TempU
{
	float Data;
	u8 Buf[4];
}FloatU;

//4�ֽ�ת���ɸ�����
float BytesToFloat(u8* data,u16 mode)
{
	float res;
	u8 temp[4];
	switch(mode)
	{
		case 4321:
			memcpy(&FloatU.Buf[0],data,4);
			break;
		case 3412:
			temp[0] = data[1];
			temp[1] = data[0];
			temp[2] = data[3];
			temp[3] = data[2];
			memcpy(&FloatU.Buf[0],temp,4);
			break;
	}
	res = FloatU.Data;
	return res;
}

//2��u16��ת���ɸ�����
float Words2Float(u16* data,u16 mode)
{
	float res;
	u8 temp[4];
	temp[0] =  (data[0]&0xFF00)>>8;
	temp[1] =  data[0]&0xFF;
	temp[2] =  (data[1]&0xFF00)>>8;
	temp[3] =  data[1]&0xFF;
	
	res = BytesToFloat(temp,mode);
	return res;
}

//1��RTU��ʽ���ַ���ת����2��Ascii��ʽ���ַ�,����RTU��0x03,���ASCII��ʽ��0x30,0x33
u16 RTU2ASCII(u8 data)
{
	u8 temp_H,temp_L;
	u16 res;
	temp_H = (data&0xF0)>>4;
	temp_L = data&0x0F;
	res = (Hex2Ascii(temp_H)<<8)|Hex2Ascii(temp_L);
	return res;
}

//����ASCIIͨ��Э���LRC���㺯��
u16 LRC(u8 *rtu_data,u16 rtu_len)
{
	// 3A 30 31 30 33 31 38 39 41 30 30 30 35 34 35 0D 0A   :0103189A000545.
	int i;
	u16 sum=0;
	u16 res;
	for(i=0;i<rtu_len;i++)
	 sum += rtu_data[i];
	
	sum = sum%256;
	sum = 256-sum;
	sum = sum%256;
	
	res = RTU2ASCII(sum);
	return res;
}


//�ȱ�������CSУ������㣬
//���е���Ч������ӣ���256��ģ����ֻȡ��8λ
u8 CS_Calc(u8 *data,u16 len)
{
	u8 res;
	u16 temp=0;
	int i;
	for(i=0;i<len;i++)
		temp += *(data+i);
	
	res = temp&0xFF;
	return res;
}

/*************************************************************************************************************
*���ܣ���ʮ����ת��ΪBCD�룬��1234ת��Ϊ0x12,0x34��2���ֽڣ���ȻҲ�ɽ�123456ת��Ϊ0x12,0x34,0x56��3���ֽ�
*������dec-ʮ������
*			 *bcd-ת����bcd��ָ��
*			 len-ת��Ϊbcd����ֽ���,���len������bcd����ǰ�油0�����罫4321ת��Ϊ3��bcd�ֽڣ���Ϊ0x00 0x12 0x34
����-��
*************************************************************************************************************/
void DEC2BCD(u16 dec,u8 *bcd,u8 len)
{
	int i;
	int data = dec;
	int temp;
	for(i=len-1;i>=0;i--)
	{
		temp = data%100;
		bcd[i] = ((temp/10)<<4) + ((temp%10)&0x0F);
		data /=100;
	}
}

/*************************************************************************************************************
*���ܣ���1�ֽ�BCD��ת��Ϊ1�ֽ�10���ƣ���BCD��0x12ת��Ϊ12
*������bcd-BCD������

����-ʮ������
*************************************************************************************************************/

u8 BCD2DEC(u8 bcd)
{
	u8 res;
	res = ((bcd&0xF0)>>4)*10+(bcd&0x0F);
	return res;
}


/*************************************************************************************************************
*									BinarySearchInterval(float *array,u16 len,float T)
*���ܣ����ַ����Ҹ�����T������array�����������䷶Χ����ֵ��
*			 ����array[]={0.0,1.0,2.0,3.0};,��0.5��(0,1)֮�䣬���ǵ�1�����䣬1.5��(1,2)֮�䣬���ǵ�2�����䣬
*																			2��(1,2]֮�䣬Ҳ�ڵ�2������
*������array-��������
*			 len-���鳤��
*			 T-Ҫ��ĸ�����
*
����:��������ֵ
*************************************************************************************************************/
u16 BinarySearchInterval(float *array,u16 len,float T)
{
	u16 low,mid,high,index=0;
	low = 0;
	high = len-1;
	if((len>1)&&(T>=array[0])&&(T<=array[len-1]))
	{	
		while(low<=high)
		{
			mid = (low+high)/2;
			if(mid==low)
			{
				index = mid+1;
				break;
			}	  
			else
			{ 
				if(array[mid]<T)
					low = mid;
				else if(array[mid]>T)
					high = mid;
				else 
				{
					index = mid;
					break;
				}
			
			} 
		}
	}
	return index;
}

/***************************************************************
*				FloatEqual(float f1,float f2)
* if f1 == f2, return 1
*			   else return 0.
*
****************************************************************/
u8 FloatEqual(float f1,float f2)
{
	int a,b;
	a = ( int )(f1*1000);
	b = ( int )(f2*1000);
	if(a == b){
		return 1;
	}      	
	return 0;
}


u16  TwoByte2Word(u8 *byte)
{
  u16 res = 0;
  res = ( u16 )((byte[0] << 8)|byte[1]);
  return res;
}

/*
**************************************************************************
*														PressCalc
*
*	����	��ѹ�����㺯��
*
*	����	��type			ѹ�������ͣ�0-��ѹ��
*																1-������ 4-20mA
*															  2-������
*					data			�ɼ��õ���ģ�������ݣ���Ҫ�����������
*
*					range			����
*
*	����	���������ѹ��ֵ
*
*************************************************************************
*/
float PressCalc( u8 type, float data, float range )
{
	float res = 0.0;
	float k,b;
	if(type > 2)
		type = 0;
	
	switch( type )
	{
		case 0:					//��ѹ��,��ʱ��Ϊ��ѹ�;�Ϊ0-5V
				k = range/5.0;
			  res = k*data;
			break;
		
		case 1:					//�����ͣ���ʱ��Ϊ������Ϊ4-20mA
			k = range/(20-4);
			b = 0 - 4*k;
			res = k*data + b;
			break;
		
		case 2:
			break;
	}
	return res;
}


/*
**************************************************************************
*														TemperatureCalc
*
*	����	���¶Ȼ��㺯����Ĭ���¶ȱ�����Ϊ4-20mA
*
*	����	��data			�ɼ��õ���ģ�������ݣ���Ҫ�����������
*
*					range_min		������Сֵ
*		
*					range_max		�������ֵ
*
*
*	����	����������¶�ֵ
*
*************************************************************************
*/
float TemperatureCalc( float data, float range_min ,float range_max )
{
	float res = 0.0;
	float k,b;
	
	if( range_max <= range_min )
		return 0.0;
	
	k = (range_max - range_min)/(20 - 4);    // 150/16
	b = range_min - 4*k;
	res = k*data + b;
	return res;
}

/*
**************************************************************************
*														WaterLevelCalc
*
*	����	��Һλ���㺯����Ĭ��Һλ������Ϊ4-20mA
*
*	����	��data			�ɼ��õ���ģ�������ݣ���Ҫ�����������
*
*		
*					range     ����
*
*
*	����	���������Һλֵ
*
*************************************************************************
*/
float WaterLevelCalc( float data, float range )
{
	float res = 0.0;
	float k,b;
	
	if( range <= 0 )
		return 0.0;
	
	k = range/(20 - 4);
	b = 0 - 4*k;
	res = k*data + b;
	return res;
}


/*
**************************************************************************
*														ValveCalc
*
*	����	�����ſ��ȷ�����Ĭ�ϱ��������Ϊ4-20mA
*
*	����	��data			�ɼ��õ���ģ�������ݣ���Ҫ�����������
*
*		
*					range     ����
*
*
*	����	��������ķ��ţ����Χ0-100
*
*************************************************************************
*/
u16 ValveCalc( float data, float range )
{
	u16 res = 0;
	float k,b;
	float temp;
	
	if( range <= 0 )
		return 0.0;
	
	k = range/(20 - 16);
	b = 0 - 4*k;
	temp = k*data + b;
	res = ( u16 )temp;
	return res;
}


void Bytes2Words(u16 *des, u8 *src)
{
	des[0] = (src[0]<<8)|src[1];
	des[1] = (src[2]<<8)|src[3];
}



/*
**************************************************************************
*														Words2Bytes
*
*	����	���������ת�����������ֽ�
*
*	����	��words  Ҫת������ָ��
*
*					bytes  ת������ֽ�ָ��
*					
*					len		 Ҫת�����ֳ���
*
*	����	����
*
*************************************************************************
*/
void Words2Bytes(u16 *words, u8 *bytes, u16 len)
{
	int i;
	for(i=0;i<len;i++)
	{
		bytes[i*2] = (words[i]&0xFF00)>>8;
		bytes[i*2+1] = words[i]&0xFF;
	}
}




/*
**************************************************************************
*														Words2Bytes
*                       HL_LimitJudge
*       
*	����	���������жϣ������������ޣ��������ޣ�С�����ޣ���������ֵ
*
*	����	��data Ҫ�Ƚϵ���
*                 
*                 high_limit   ����ֵ
*
*                 low_limit    ����ֵ
*
*
*	����	���ȽϺ��ֵ
*
*************************************************************************
*/
float HL_LimitJudge(float data, float low_limit, float high_limit)
{
	float res = data;
	if(data > high_limit)
		res = high_limit;

	if(data < low_limit)
		res = low_limit;
	return res;
}

//���0-100��0-50��Ӧ4-20mA
//tyep = 0  ִ����Ϊ�綯������ΧΪ0-100�������ϵ��y = 0.16x + 4��x-���ֵ��y-����ֵ
//type = 1  ִ����Ϊ�����ã���ΧΪ0-50�������ϵΪ��y = 0.32x + 4��x-���ֵ��y-����ֵ
float Output2mA(float data, u16 type)
{
  float res = 0.0;
  if(type == 0)
  	res = 0.16*data + 4;
  else 
  	res = 0.32*data + 4;
 
  res = (res > 21.0) ? 21:res;
  res = (res < 0.0) ? 0.0:res;
  return res;
}


/*
 * ���ڱ���ֵ��������������������������
 * 
 * @para[in]   original_value  -- ����ԭʼֵ
 * @para[in]   zero_k		   -- ���У��
 * @para[in]   full_k		   --  ������У��
 * 
 * @return     ������ı���ֵ
 * 
 */
float variable_correction_calc(float original_value, float zero_k, float full_k)
{
	float res;
	if(original_value < 0){
		return 0.0;
	}

	res = original_value*(1 + full_k) + zero_k;
	return res;
}



u8 FloatIntervalJudge(float data, float seg1, float seg2)
{
        u8 res = 0;
	u32 idata,iseg1;
	if((data > seg1) && (data < seg2))
		res = 1;
	else if(data < seg1)
		res = 0;
	else if(data > seg2)
		res = 0;
	else
	{
		idata = ( u32 )(fabs(data)*1000);
		iseg1 = ( u32 )(fabs(seg1)*1000);
		if(idata == iseg1)
		  res = 1;
	}
	return res;
}

/*
****************************************************************************
*       
*               ���������ж�
*
* �������ú�����Ҫ������Ҫ������������Ҫ��֤data1~4Ϊ������ϵ
*       �� data1 < data2 < data3 < data4
*
* ���أ�0-��ʾ��ϵ��ȷ
*       n-��n������������
*
*       
****************************************************************************
*/
u8 Para_IncJudge(float data1, float data2, float data3, float data4)
{
	u8 res = 0;
	if((data1 < data2) && (data2 < data3) && (data3 < data4))
		return 0;
	else
	{
		if(data1 > data2)
			return 1;                       //����Ҫ���̷��أ���Ϊ������������Ͳ����ر��Ͻ����Ǽ���ĳ��ֻ����1�εĴ�λ�趨
		if((data2 < data1) || (data2 > data3))
			return 2;
		if((data3 < data2) || (data3 > data4))
			return 3;
		if((data4 < data3) || (data4 < data2) || (data4 < data1))
			return 4;
	}
	return res;
}



int binary_search_int16t(int16_t *num, int num_cnt, int16_t target)
{
	int first = 0, last = num_cnt - 1;
	int mid;

	if(target <= num[0]){
		return 0;
	}

	if(target >= num[num_cnt - 1]){
		return num_cnt - 1;
	}

	while(first <= last){
		mid = (first + last)/2;
		if(num[mid] > target){
			last = mid - 1;
		}else if(num[mid] < target){
			first = mid + 1;
		}else{
			return mid;
		}
	}
    return 0;
} 