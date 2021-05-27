/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

#include "port.h"
#include "rs485_1.h"	
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"
/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
	if(TRUE==xRxEnable)
	{
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
	else
	{
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);	
	}

	if(TRUE==xTxEnable)
	{
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	}
	else
	{
	   USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	}
}

void
vMBPortClose( void )
{
	USART_ITConfig(USART1, USART_IT_TXE|USART_IT_RXNE, DISABLE);
	USART_Cmd(USART1, DISABLE);
}

BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	switch(eParity)
	{
		case MB_PAR_NONE:
			RS485_1st_Init(ulBaudRate,ucDataBits,0);
		 break;	
		case MB_PAR_ODD:
			RS485_1st_Init(ulBaudRate,ucDataBits,1);
		 break;	
		case MB_PAR_EVEN:
			RS485_1st_Init(ulBaudRate,ucDataBits,2);
		 break;	
	}
	return TRUE;
}



BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
	/*
	USART_SendData(USART1, ucByte);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {
  
  }	
	*/	
	RS485_1st_SendByte(ucByte);
	return TRUE;
}

BOOL
xMBPortSerialGetByte(CHAR * pucByte )
{  
  
    if((MB_RTU_ENABLED > 0) && (MB_ASCII_ENABLED == 0))
	*pucByte = USART_ReceiveData(USART1);
    else if((MB_RTU_ENABLED == 0) && (MB_ASCII_ENABLED > 0))
        *pucByte = USART_ReceiveData(USART1)&0x7F;
	
    return TRUE;
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
void prvvUARTTxReadyISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
void prvvUARTRxISR(void)
{
    pxMBFrameCBByteReceived();
}
