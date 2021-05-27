#include "sim_i2c.h"
#include "delay.h"
#include "os_cpu.h"

/*
******************************************
***ADC1
*  SCL--PE6
*  SDA--PE5

***AT24C16
*  SCL--PE0
*  SDA--PE1
******************************************
*/

void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	
  //ADC1	   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6); 

  // RX8025T
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_5|GPIO_Pin_8);
}

void ADC1_IIC_Start(void)
{
    ADC1_SDA_OUT();     //
    ADC1_IIC_SDA=1;	  	  
    ADC1_IIC_SCL=1;
    delay_us(2);
    ADC1_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(2);
    ADC1_IIC_SCL=0;//ǯסI2C
}


void ADC1_IIC_Stop(void)
{
    ADC1_SDA_OUT();//
    ADC1_IIC_SCL=0;
    ADC1_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(2);
    ADC1_IIC_SCL=1; 
    ADC1_IIC_SDA=1;//
    delay_us(2);	
}

void ADC1_IIC_SendByte(u8 byte)
{
    u8 t;   
    ADC1_SDA_OUT(); 	    
    ADC1_IIC_SCL=0;//
    for(t=0;t<8;t++)
    {              
        //ADC1_IIC_SDA=(txd&0x80)>>7;
	if((byte&0x80)>>7)
          ADC1_IIC_SDA=1;
	else
          ADC1_IIC_SDA=0;
	byte<<=1; 	  
	delay_us(2);   //
	ADC1_IIC_SCL=1;
	delay_us(2); 
	ADC1_IIC_SCL=0;	
	delay_us(2);
    }	 
}

//
u8 ADC1_IIC_ReadByte(u8 ack)
{
    u8 i,receive=0;
    ADC1_SDA_IN();//
    for(i=0;i<8;i++ )
    {
        ADC1_IIC_SCL=0; 
        delay_us(2);
	ADC1_IIC_SCL=1;
        receive<<=1;
        if(ADC1_READ_SDA)receive++;   
		
        delay_us(1); 
    }					 
    if (!ack)
      ADC1_IIC_NAck();//
    else
      ADC1_IIC_Ack(); //
    return receive;
}

u8 ADC1_IIC_WaitAck(void)
{
    u8 ucErrTime=0;
    ADC1_SDA_IN();      //
    ADC1_IIC_SDA=1;delay_us(1);	   
    ADC1_IIC_SCL=1;delay_us(1);	 
    while(ADC1_READ_SDA)
    {
	ucErrTime++;
	if(ucErrTime>250)
	{
          ADC1_IIC_Stop();
          return 1;
         }
    }
    ADC1_IIC_SCL=0;//	   
    return 0;  
}


//
void ADC1_IIC_Ack(void)
{
    ADC1_IIC_SCL=0;
    ADC1_SDA_OUT();
    ADC1_IIC_SDA=0;
    delay_us(2);
    ADC1_IIC_SCL=1;
    delay_us(2);
    ADC1_IIC_SCL=0;
}


void ADC1_IIC_NAck(void)
{
    ADC1_IIC_SCL=0;
    ADC1_SDA_OUT();
    ADC1_IIC_SDA=1;
    delay_us(2);
    ADC1_IIC_SCL=1;
    delay_us(2);
    ADC1_IIC_SCL=0;
}


/*******************EEPROM-IIC����*****************************/
void EEPROM_IIC_Start(void)
{
    EEPROM_SDA_OUT();     //sda�����??
    EEPROM_IIC_SDA=1;	  	  
    EEPROM_IIC_SCL=1;
    delay_us(4);
    EEPROM_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    EEPROM_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������?? 
}


void EEPROM_IIC_Stop(void)
{
    EEPROM_SDA_OUT();//sda�����??
    EEPROM_IIC_SCL=0;
    EEPROM_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    EEPROM_IIC_SCL=1; 
    EEPROM_IIC_SDA=1;//����I2C���߽����ź�
    delay_us(4);	
}

void EEPROM_IIC_SendByte(u8 byte)
{
    u8 t;   
    EEPROM_SDA_OUT(); 	    
    EEPROM_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //EEPROM_IIC_SDA=(txd&0x80)>>7;
	if((byte&0x80)>>7)
          EEPROM_IIC_SDA=1;
	else
          EEPROM_IIC_SDA=0;
	byte<<=1; 	  
	delay_us(2);   //��TEA5767��������ʱ���Ǳ����??
	EEPROM_IIC_SCL=1;
	delay_us(2); 
	EEPROM_IIC_SCL=0;	
	delay_us(2);
    }	 
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK  
u8 EEPROM_IIC_ReadByte(u8 ack)
{
    u8 i,receive=0;
    EEPROM_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        EEPROM_IIC_SCL=0; 
        delay_us(2);
	EEPROM_IIC_SCL=1;
        receive<<=1;
        if(EEPROM_READ_SDA)receive++;   
		
        delay_us(1); 
    }					 
    if (!ack)
      EEPROM_IIC_NAck();//����nACK
    else
      EEPROM_IIC_Ack(); //����ACK   
    return receive;
}

u8 EEPROM_IIC_WaitAck(void)
{
    u8 ucErrTime=0;
    EEPROM_SDA_IN();      //SDA����Ϊ����  
    EEPROM_IIC_SDA=1;delay_us(1);	   
    EEPROM_IIC_SCL=1;delay_us(1);	 
    while(EEPROM_READ_SDA)
    {
	ucErrTime++;
	if(ucErrTime>250)
	{
          EEPROM_IIC_Stop();
          return 1;
         }
    }
    EEPROM_IIC_SCL=0;//ʱ�����??0 	   
    return 0;  
}


//����ACKӦ��
void EEPROM_IIC_Ack(void)
{
    EEPROM_IIC_SCL=0;
    EEPROM_SDA_OUT();
    EEPROM_IIC_SDA=0;
    delay_us(2);
    EEPROM_IIC_SCL=1;
    delay_us(2);
    EEPROM_IIC_SCL=0;
}

//����NAckӦ��
void EEPROM_IIC_NAck(void)
{
    EEPROM_IIC_SCL=0;
    EEPROM_SDA_OUT();
    EEPROM_IIC_SDA=1;
    delay_us(2);
    EEPROM_IIC_SCL=1;
    delay_us(2);
    EEPROM_IIC_SCL=0;
}


/*---------------------------------------------------------------*/
void RX8025_IIC_Start(void)
{
    RX8025_SDA_OUT();     //sda�����??
    RX8025_IIC_SDA=1;	  	  
    RX8025_IIC_SCL=1;
    delay_us(4);
    RX8025_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
    delay_us(4);
    RX8025_IIC_SCL=0;//
}


void RX8025_IIC_Stop(void)
{
    RX8025_SDA_OUT();//sda�����??
    RX8025_IIC_SCL=0;
    RX8025_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    delay_us(4);
    RX8025_IIC_SCL=1; 
    RX8025_IIC_SDA=1;//����I2C���߽����ź�
    delay_us(4);	
}

void RX8025_IIC_SendByte(u8 byte)
{
    u8 t;   
    RX8025_SDA_OUT(); 	    
    RX8025_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        //RX8025_IIC_SDA=(txd&0x80)>>7;
	if((byte&0x80)>>7)
          RX8025_IIC_SDA=1;
	else
          RX8025_IIC_SDA=0;
	byte<<=1; 	  
	delay_us(2);   //��TEA5767��������ʱ���Ǳ����??
	RX8025_IIC_SCL=1;
	delay_us(2); 
	RX8025_IIC_SCL=0;	
	delay_us(2);
    }	 
}

// ack = 1, send ack;
// ack = 0, send nAck; 
u8 RX8025_IIC_ReadByte(u8 ack)
{
    u8 i,receive=0;
    RX8025_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
    {
        RX8025_IIC_SCL=0; 
        delay_us(2);
	RX8025_IIC_SCL=1;
        receive<<=1;
        if(RX8025_READ_SDA)receive++;   
		
        delay_us(1); 
    }					 
    if (!ack)
      RX8025_IIC_NAck();//����nACK
    else
      RX8025_IIC_Ack(); //����ACK   
    return receive;
}

u8 RX8025_IIC_WaitAck(void)
{
    u8 ucErrTime=0;
    RX8025_SDA_IN();      //SDA����Ϊ����  
    RX8025_IIC_SDA=1;delay_us(1);	   
    RX8025_IIC_SCL=1;delay_us(1);	 
    while(RX8025_READ_SDA)
    {
	ucErrTime++;
	if(ucErrTime>250)
	{
          RX8025_IIC_Stop();
          return 1;
         }
    }
    RX8025_IIC_SCL=0;//ʱ�����??0 	   
    return 0;  
}


//����ACKӦ��
void RX8025_IIC_Ack(void)
{
    RX8025_IIC_SCL=0;
    RX8025_SDA_OUT();
    RX8025_IIC_SDA=0;
    delay_us(2);
    RX8025_IIC_SCL=1;
    delay_us(2);
    RX8025_IIC_SCL=0;
}

//����NAckӦ��
void RX8025_IIC_NAck(void)
{
    RX8025_IIC_SCL=0;
    RX8025_SDA_OUT();
    RX8025_IIC_SDA=1;
    delay_us(2);
    RX8025_IIC_SCL=1;
    delay_us(2);
    RX8025_IIC_SCL=0;
}
