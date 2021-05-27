

#include "port2serial.h"
#include "rs485_2.h"
#include "mb2rtu.h"



extern          u8( *pxMB2FrameCBTransmitterEmpty ) ( void );
extern          u8( *pxMB2FrameCBByteReceived ) ( void );

/* ----------------------- Start implementation -----------------------------*/
void vMB2PortSerialEnable( u8 xRxEnable, u8 xTxEnable )
{
	if(1==xRxEnable)
	{
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	
	}

	if(1==xTxEnable)
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	}
	else
	{
	   USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
	}
}

void
vMB2PortClose( void )
{
	USART_ITConfig(USART2, USART_IT_TXE|USART_IT_RXNE, DISABLE);
	USART_Cmd(USART2, DISABLE);
}

u8
xMB2PortSerialInit( u8 ucPORT, u32 ulBaudRate, u8 ucDataBits, u8 eParity )
{
	
    RS485_2nd_Init(ulBaudRate,ucDataBits,eParity);
		
    return 1;
}



u8
xMB2PortSerialPutByte( u8 ucByte )
{
	/*
	USART_SendData(USART2, ucByte);
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {
  
  }	
	*/	
	RS485_2nd_SendByte(ucByte);
	return 1;
}

u8
xMB2PortSerialGetByte(u8 * pucByte )
{  
  
   // if((MB2_RTU_ENABLED > 0) && (MB2_ASCII_ENABLED == 0))
	*pucByte = USART_ReceiveData(USART2);
    //else if((MB2_RTU_ENABLED == 0) && (MB2_ASCII_ENABLED > 0))
     //   *pucByte = USART_ReceiveData(USART2)&0x7F;
	
    return 1;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMB2FrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMB2PortSerialPutByte( ) to send the character.
 */
void prvvUART2TxReadyISR(void)
{
   pxMB2FrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMB2FrameCBByteReceived( ). The
 * protocol stack will then call xMB2PortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUART2RxISR(void)
{
    pxMB2FrameCBByteReceived();
}
