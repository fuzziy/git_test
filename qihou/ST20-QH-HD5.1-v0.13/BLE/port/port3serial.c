

#include "port3serial.h"
#include "rs485_3.h"	



extern          u8( *pxMB3FrameCBTransmitterEmpty ) ( void );
extern          u8( *pxMB3FrameCBByteReceived ) ( void );

/* ----------------------- Start implementation -----------------------------*/
void vMB3PortSerialEnable( u8 xRxEnable, u8 xTxEnable )
{
	if(1==xRxEnable)
	{
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);	
	}

	if(1==xTxEnable)
	{
		USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
	}
	else
	{
	   USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
	}
}

void
vMB3PortClose( void )
{
	USART_ITConfig(USART3, USART_IT_TXE|USART_IT_RXNE, DISABLE);
	USART_Cmd(USART3, DISABLE);
}

u8
xMB3PortSerialInit( u8 ucPORT, u32 ulBaudRate, u8 ucDataBits, u8 eParity )
{
	
    RS485_3rd_Init(ulBaudRate,ucDataBits,eParity);
		
    return 1;
}



u8
xMB3PortSerialPutByte( u8 ucByte )
{
	/*
	USART_SendData(USART3, ucByte);
  while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
  {
  
  }	
	*/	
	RS485_3rd_SendByte(ucByte);
	return 1;
}

u8
xMB3PortSerialGetByte(u8 * pucByte )
{  
  
   // if((MB3_RTU_ENABLED > 0) && (MB3_ASCII_ENABLED == 0))
	*pucByte = USART_ReceiveData(USART3);
    //else if((MB3_RTU_ENABLED == 0) && (MB3_ASCII_ENABLED > 0))
     //   *pucByte = USART_ReceiveData(USART3)&0x7F;
	
    return 1;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMB3FrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMB3PortSerialPutByte( ) to send the character.
 */
void prvvUART3TxReadyISR(void)
{
   pxMB3FrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMB3FrameCBByteReceived( ). The
 * protocol stack will then call xMB3PortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUART3RxISR(void)
{
    pxMB3FrameCBByteReceived();
}
