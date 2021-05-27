#define MY_MODBUS_GLOBALS
#include "my_modbus.h"
#include "wrappers.h"
#include "mb.h"
#include "24cxx.h"
#include "rx8025.h"


u16   usRegInputStart = REG_INPUT_START;
u16   usRegInputBuf[REG_INPUT_NREGS];
u16   usRegHoldingStart = REG_HOLDING_START;
u16   usRegHoldingBuf[REG_HOLDING_NREGS];



/****************************************************************************
* ��	  �ƣ�eMBRegInputCB 
* ��    �ܣ���ȡ����Ĵ�������Ӧ�������� 04 eMBFuncReadInputRegister
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNRegs: Ҫ��ȡ�ļĴ�������
* ���ڲ�����
* ע	  �⣺��λ�������� ֡��ʽ��: SlaveAddr(1 Byte)+FuncCode(1 Byte)
*								+StartAddrHiByte(1 Byte)+StartAddrLoByte(1 Byte)
*								+LenAddrHiByte(1 Byte)+LenAddrLoByte(1 Byte)+
*								+CRCAddrHiByte(1 Byte)+CRCAddrLoByte(1 Byte)
*							3 ��
****************************************************************************/
eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( UCHAR )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

/****************************************************************************
* ��	  �ƣ�eMBRegHoldingCB 
* ��    �ܣ���Ӧ�������У�06 д���ּĴ��� eMBFuncWriteHoldingRegister 
*													16 д������ּĴ��� eMBFuncWriteMultipleHoldingRegister
*													03 �����ּĴ��� eMBFuncReadHoldingRegister
*													23 ��д������ּĴ��� eMBFuncReadWriteMultipleHoldingRegister
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNRegs: Ҫ��д�ļĴ�������
*						eMode: ������
* ���ڲ�����
* ע	  �⣺4 ��
****************************************************************************/
eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;
	int 		index;						//�Ĵ����������ʼ��ַ����Ե�ַ
	u16		nRegs = usNRegs;	//Ҫ��/д�ļĴ�������.
	
	if((usAddress >= REG_HOLDING_START)&&\
		((usAddress+usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		index = iRegIndex;			//��ȡҪ��/д���������ʼ��ַ�ļĴ�����ַ��
		switch(eMode)
		{                                       
                  case MB_REG_READ://�� MB_REG_READ = 0
                    while(usNRegs > 0)
                    {
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);            
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF); 
                        iRegIndex++;
                        usNRegs--;					
                    }                            
                    break;
                  case MB_REG_WRITE://д MB_REG_WRITE = 0
                    while(usNRegs > 0)
                    {         
			usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                        usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                        iRegIndex++;
                        usNRegs--;
                     }	
                    //д���ּĴ�����Ӧ�������±��ּĴ�����Ӧ�Ĺ����������Ҵ洢��Flash��
                    Modbus_UpdateEngineValue(index,nRegs);
                    ///////////////////////////////
                  break;
		}
	}
	else//����
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}

/****************************************************************************
* ��	  �ƣ�eMBRegCoilsCB 
* ��    �ܣ���Ӧ�������У�01 ����Ȧ eMBFuncReadCoils
*													05 д��Ȧ eMBFuncWriteCoil
*													15 д�����Ȧ eMBFuncWriteMultipleCoils
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: ��Ȧ��ַ
*						usNCoils: Ҫ��д����Ȧ����
*						eMode: ������
* ���ڲ�����
* ע	  �⣺��̵��� 
*						0 ��
****************************************************************************/
eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
	eMBErrorCode    eStatus = MB_ENOERR;
	int             iRegIndex;


	if((usAddress >= REG_HOLDING_START)&&\
		((usAddress+usNCoils) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		switch(eMode)
		{                                       
			case MB_REG_READ://�� MB_REG_READ = 0
        while(usNCoils > 0)
				{
// 					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);            
// 					*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF); 
          iRegIndex++;
          usNCoils--;					
				}                            
        break;
			case MB_REG_WRITE://д MB_REG_WRITE = 1
				while(usNCoils > 0)
				{         
          // usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
          //usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
          iRegIndex++;
          usNCoils--;
        }				
			}
	}
	else//����
	{
		eStatus = MB_ENOREG;
	}	
	
	return eStatus;
}
/****************************************************************************
* ��	  �ƣ�eMBRegDiscreteCB 
* ��    �ܣ���ȡ��ɢ�Ĵ�������Ӧ�������У�02 ����ɢ�Ĵ��� eMBFuncReadDiscreteInputs
* ��ڲ�����pucRegBuffer: ���ݻ�������������Ӧ����   
*						usAddress: �Ĵ�����ַ
*						usNDiscrete: Ҫ��ȡ�ļĴ�������
* ���ڲ�����
* ע	  �⣺1 ��
****************************************************************************/
eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    ( void )pucRegBuffer;
    ( void )usAddress;
    ( void )usNDiscrete;
    return MB_ENOREG;
}



void Modbus_UpdateRegHolding(void)
{
	
}


static void modbus_set_time(void)
{
  rt_time_t time;
  time.year = usRegHoldingBuf[MB_INDEX_CLOCK_YEAR] > 2000 ? (usRegHoldingBuf[MB_INDEX_CLOCK_YEAR] - 2000) : (u8)usRegHoldingBuf[MB_INDEX_CLOCK_YEAR];
  time.month = usRegHoldingBuf[MB_INDEX_CLOCK_MONTH] < 13 ? usRegHoldingBuf[MB_INDEX_CLOCK_MONTH] : 1;
  time.day = usRegHoldingBuf[MB_INDEX_CLOCK_DAY] < 32 ? usRegHoldingBuf[MB_INDEX_CLOCK_DAY] : 1;
  time.week = 1;
  time.hour = usRegHoldingBuf[MB_INDEX_CLOCK_HOUR] < 24 ? usRegHoldingBuf[MB_INDEX_CLOCK_HOUR] : 0;
  time.min = usRegHoldingBuf[MB_INDEX_CLOCK_MIN] < 60 ? usRegHoldingBuf[MB_INDEX_CLOCK_MIN] : 0;
  time.sec = usRegHoldingBuf[MB_INDEX_CLOCK_SEC] < 60 ? usRegHoldingBuf[MB_INDEX_CLOCK_SEC] : 0;
  rx8025t_set_time(&time);
}
/*
*************************************************************************							
*						Modbus_UpdateEngineValue( int index, u16 nRegs )
*
*
*/
void Modbus_UpdateEngineValue( int index, u16 nRegs )
{
  u8 buf[21];
  u16 ee_addr,ee_bytes;
  u16 i;
  if((index >= MB_INDEX_START_SAVE) &&
     (index <= MB_INDEX_END_SAVE)){
      if((index >= MB_INDEX_CLOCK_YEAR) && (index <= MB_INDEX_CLOCK_SEC)){
        modbus_set_time();
      }else{
        if(nRegs <= 10){
          ee_addr = (index - MB_INDEX_START_SAVE)*2 + 2;
          ee_bytes = nRegs*2;
          for(i = 0; i < nRegs; i++){
            buf[i*2] = ( u8 )(usRegHoldingBuf[index + i] >> 8);
            buf[i*2 + 1] = ( u8 )(usRegHoldingBuf[index + i]&0xFF);
          }

          if((ee_addr + ee_bytes - 1) < (EE_PARA_NBYTES - 2)){
            q_AT24CXX_Write(ee_addr, buf, ee_bytes);
          }
        }
      }
  }
}
