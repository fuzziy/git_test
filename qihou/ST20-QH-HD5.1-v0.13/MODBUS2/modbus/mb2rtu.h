#include "stm32f10x.h"

#ifndef _MB2_RTU_H
#define _MB2_RTU_H


#include "mb2.h"

//debug
extern u8 Flag_USART2_RX;
//debug



eMB2ErrorCode
eMB2RTUInit( u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity );

void
eMB2RTUStart( void );

u8      xMB2RTUReceiveFSM( void );

eMB2ErrorCode 
eMB2RTUReceive( u8 * pucRcvAddress, u8 ** pucFrame, u16 * pusLength );

eMB2ErrorCode
eMB2RTUSend( u8 ucSlaveAddress, const u8 * pucFrame, u16 usLength );

u8	xMB2RTUTransmitFSM( void );

u8      xMB2RTUTimerT35Expired( void );



eMB2Exception   MB2Error2Exception( eMB2ErrorCode eErrorCode );

eMB2Exception   eMB2FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen );


eMB2Exception   eMB2FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen );


void RS485_2nd_Init(u32 BaudRate, u8 DataBits, u8 Parity);

#endif