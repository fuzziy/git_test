#include "24cxx.h"
#include "sim_i2c.h"
#include "delay.h"


//AT24C16 ҳд������д���ֽڳ��Ȳ��ܳ���EEPROM��1ҳ
#define AT24CXX_SLAVE_ADDR			0xA0
#define AT24CXX_CMD_W				0x00
#define AT24CXX_CMD_R				0x01
void AT24CXX_PageWrite(u16 write_addr, u8 *buf, u8 write_len)
{
	u8 i;
	u8 len = write_len;
	u8 device_address,page_address,word_address;
	if(write_len > AT24C16_PAGESIZE)
		len = AT24C16_PAGESIZE;
	page_address = ((write_addr/256) << 1);
	device_address = AT24CXX_SLAVE_ADDR | page_address |AT24CXX_CMD_W;
	word_address = write_addr & 0xFF;
	EEPROM_IIC_Start();
	EEPROM_IIC_SendByte(device_address);
	EEPROM_IIC_WaitAck();
	EEPROM_IIC_SendByte(word_address);
        EEPROM_IIC_WaitAck();
	for(i=0;i<len;i++)
	{
		EEPROM_IIC_SendByte(buf[i]);
		EEPROM_IIC_WaitAck();
				
	}
	EEPROM_IIC_Stop();
        delay_ms(10);   //�ǳ���Ҫ��ÿ��stop֮��stop��start֮�����Ҫ��ʱ����
}

void AT24CXX_RandomRead(u16 read_addr, u8 *buf, u16 read_len)
{
	u8 i;
	u16 len = read_len;
	u8 device_address,page_address,word_address;
	
	page_address = ((read_addr/256) << 1);
	device_address = AT24CXX_SLAVE_ADDR | page_address;
	word_address = read_addr & 0xFF;

	EEPROM_IIC_Start();
	EEPROM_IIC_SendByte(device_address|AT24CXX_CMD_W);
	EEPROM_IIC_WaitAck();
	EEPROM_IIC_SendByte(word_address);
	EEPROM_IIC_WaitAck();
	EEPROM_IIC_Start();
	EEPROM_IIC_SendByte(device_address|AT24CXX_CMD_R);
	EEPROM_IIC_WaitAck();
	for(i=0;i<len-1;i++)
	{
		buf[i] = EEPROM_IIC_ReadByte(1);
	}
	buf[i] = EEPROM_IIC_ReadByte(0);
	EEPROM_IIC_Stop();
        //delay_ms(10);   //�ǳ���Ҫ��ÿ��stop֮��stop��start֮�����Ҫ��ʱ����
}

#define AT24CXX_PAGESIZE  16
void AT24C16_WriteBytes(u16 write_addr, u8 *pwrite_buff, u16 writebytes)
{
        u8   write_len,page_offset;
	while(writebytes > 0)
	{
		page_offset = AT24CXX_PAGESIZE - (write_addr % AT24CXX_PAGESIZE); //EE24CLXX_PAGESIZEΪҳ��С����24c16Ϊ16
		write_len   = writebytes > page_offset ? page_offset : writebytes;
		AT24CXX_PageWrite(write_addr,pwrite_buff, write_len);		   //дһҳ����
		writebytes   = writebytes - write_len;
		if(writebytes > 0)
		{
			pwrite_buff = pwrite_buff + write_len;
			write_addr  = write_addr + write_len;
		}
	}
}

/*****************************************************************
*������: AT24CXX_Write_Bytes(u8 *pBuffer,u16 WriteAddress,u8 Len)
*����: ҳд���� ���д��һҳ(16�ֽ�)
*����: �ײ�I2Cд����
*������:�ⲿ����
*�β�:
      *pBuffer��ָ��д�뻺����
			WriteAddr��Ҫд��ĵ�ַ
			Len��д�����ݳ���
*����ֵ:��
*����:����һ��I2Cʱ�����д��һҳ(16Bytes)����,���Կ��ڰ��ֽ�д��
*****************************************************************/
void q_AT24CXX_Write_Bytes(u8 *pBuffer,u16 WriteAddress,u8 Len)
{
	 unsigned char Page=0,WordAddress=0,DeviceAddress=0xA0;
    u8 i=0;
    Page=WriteAddress/AT24CXX_PAGESIZE;
    WordAddress=(WriteAddress%AT24CXX_PAGESIZE) & 0x0F;
    DeviceAddress |= (((Page<<1) & 0xE0)>>4);//High 3 bits
    WordAddress |= (Page & 0x0F)<<4;//Low 4 bits
    EEPROM_IIC_Start();
    EEPROM_IIC_SendByte(DeviceAddress);//�����豸��ַ
    EEPROM_IIC_WaitAck();
    EEPROM_IIC_SendByte(WordAddress);//�����ֽڵ�ַ
    EEPROM_IIC_WaitAck();
    for(i=0; i<Len; i++)
    {
        EEPROM_IIC_SendByte(*pBuffer++);//�����ֽڵ�ַ
        EEPROM_IIC_WaitAck();
    }
    EEPROM_IIC_Stop();//����һ��ֹͣ����
    delay_ms(10);
}


/*****************************************************************
*������: AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
*����:AT24CXX ����д�벻�����ֽ�
*����:
*������:�ⲿ����
*�β�:
			WriteAddr��Ҫд����׵�ַ
			*pBuffer��ָ��д�뻺����
			NumToWrite��д����ֽ���
*����ֵ:��
*����:����ģʽ ����ÿ��дһ���ֽھ�����һ��I2Cʱ��
*****************************************************************/
void q_AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
    unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr=WriteAddr%AT24CXX_PAGESIZE;//��ַ������16�ֽڶ���
    count=AT24CXX_PAGESIZE-Addr;//�������ֽ���
    NumOfPage=NumToWrite/AT24CXX_PAGESIZE;//��Ҫд�����ҳ
    NumOfSingle=NumToWrite%AT24CXX_PAGESIZE;//ʣ����д����ֽ���
    if(0==Addr)//�����ַ����
    {
        if(NumToWrite<=AT24CXX_PAGESIZE)//д���ֽ�<=1ҳ
        {
            q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,NumToWrite);
        }
        else
        {
            while(NumOfPage--)//��ҳд��
            {
                q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,AT24CXX_PAGESIZE);
                pBuffer+=AT24CXX_PAGESIZE;
                WriteAddr+=AT24CXX_PAGESIZE;
            }
            if(NumOfSingle != 0)//�����ʣ���ֽ�
            {
                q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,NumOfSingle);//��ʣ�µ��ֽ�д��
            }
        }
    }
    else//��ַ������
    {
        if(NumToWrite<=count)// Ҫд����ֽ���<=count
        {
            q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,NumToWrite);//д��ʵ���ֽ���
        }
        else//Ҫд���ֽ�������count
        {
            q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,count);//�ֽ�count���ֽ�д�� д��� ��ַ�պö���
            NumToWrite-=count;//����ʣ���ֽ���
            pBuffer+=count;//д������ƫ��count
            WriteAddr+=count;//д���ַƫ��count
 
            NumOfPage=NumToWrite/AT24CXX_PAGESIZE;//��Ҫд�����ҳ
            NumOfSingle=NumToWrite%AT24CXX_PAGESIZE;//ʣ����д����ֽ���
 
            while(NumOfPage--)//�Ȱ�ҳд��
            {
                q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,AT24CXX_PAGESIZE);
                pBuffer+=AT24CXX_PAGESIZE;
                WriteAddr+=AT24CXX_PAGESIZE;
            }
            if(NumOfSingle != 0)//��ʣ���ֽ�
            {
                q_AT24CXX_Write_Bytes(pBuffer,WriteAddr,NumOfSingle);//��ʣ�µ��ֽ�д��
            }
        }
    }
}


/*
 ****************************************************************
 *������: AT24CXX_ReadOneByte(u16 ReadAddr)
 *����:AT24CXX ��ָ����ַ��һ���ֽ�   AT24C16ʹ��
 *����:�ײ�I2C��д����
 *������:�ⲿ����
 *�β�:
			ReadAddr��Ҫ��ȡ�ĵ�ַ
 *����ֵ:���ض�ȡ������
 *����:����дģʽ��ÿ�ζ�������һ��I2Cʱ��
****************************************************************
*/
u8 q_AT24CXX_ReadOneByte(u16 ReadAddr)
{
    unsigned char Page=0,WordAddress=0,DeviceAddress=0xA0;
    u8 temp=0;
    Page=ReadAddr/AT24CXX_PAGESIZE;
    WordAddress=(ReadAddr%AT24CXX_PAGESIZE) & 0x0F;
    DeviceAddress |= (((Page<<1) & 0xE0)>>4);//High 3 bits
    WordAddress |= (Page & 0x0F)<<4;//Low 4 bits
    EEPROM_IIC_Start();
    EEPROM_IIC_SendByte(DeviceAddress&0xFE);//�����豸��ַ+д����
    EEPROM_IIC_WaitAck();
    EEPROM_IIC_SendByte(WordAddress);//�����ֽڵ�ַ
    EEPROM_IIC_WaitAck();
    EEPROM_IIC_Start();                //��ʼ�ź�
    EEPROM_IIC_SendByte(DeviceAddress|0x01);//�����豸��ַ+������
    EEPROM_IIC_WaitAck();
    temp = EEPROM_IIC_ReadByte(0);
    EEPROM_IIC_Stop();//����һ��ֹͣ����
    return temp;
}

/*
 ****************************************************************
 *	������: AT24CXX_ReadOneByte(u16 ReadAddr)
 *	����:AT24CXX ��ָ����ַ��һ���ֽ�   AT24C16ʹ��
 *	����:�ײ�I2C��д����
 *	������:�ⲿ����
 *	�β�:
			ReadAddr��Ҫ��ȡ�ĵ�ַ
 *	����ֵ:���ض�ȡ������
 *	����: ���ٶ�����
****************************************************************
*/
void q_AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
    unsigned char Page=0,WordAddress=0,DeviceAddress=0x50;
    Page=ReadAddr/AT24CXX_PAGESIZE;
    WordAddress=(ReadAddr%AT24CXX_PAGESIZE) & 0x0F;
    DeviceAddress |= (((Page<<1) & 0xE0)>>4);//High 3 bits
    WordAddress |= (Page & 0x0F)<<4;//Low 4 bits
    while(NumToRead)
    {
        *pBuffer++ = q_AT24CXX_ReadOneByte(ReadAddr++);
        NumToRead--;
    }
}


//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
    u8 temp=0;		  	    																 
    EEPROM_IIC_Start();  
    if(EE_TYPE>AT24C16){
	EEPROM_IIC_SendByte(0XA0);	   //����д����
	EEPROM_IIC_WaitAck();
	EEPROM_IIC_SendByte(ReadAddr>>8);//���͸ߵ�ַ
	EEPROM_IIC_WaitAck();		 
    }else{ 
      EEPROM_IIC_SendByte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	 
    }
    
    EEPROM_IIC_WaitAck(); 
    EEPROM_IIC_SendByte(ReadAddr%256);   //���͵͵�ַ
    EEPROM_IIC_WaitAck();	    
    EEPROM_IIC_Start();  	 	   
    EEPROM_IIC_SendByte(0XA1);           //�������ģʽ			   
    EEPROM_IIC_WaitAck();	 
    temp=EEPROM_IIC_ReadByte(0);		   
    EEPROM_IIC_Stop();//����һ��ֹͣ����	    
    return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    EEPROM_IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		EEPROM_IIC_SendByte(0XA0);	    //����д����
		EEPROM_IIC_WaitAck();
		EEPROM_IIC_SendByte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		EEPROM_IIC_SendByte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	EEPROM_IIC_WaitAck();	   
    EEPROM_IIC_SendByte(WriteAddr%256);   //���͵͵�ַ
	EEPROM_IIC_WaitAck(); 	 										  		   
	EEPROM_IIC_SendByte(DataToWrite);     //�����ֽ�							   
	EEPROM_IIC_WaitAck();  		    	   
    EEPROM_IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(5);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
