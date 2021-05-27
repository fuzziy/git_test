#include "stm32f10x.h"

#ifndef _MB3_RTU_H
#define _MB3_RTU_H


#include "mb3.h"


eMB3ErrorCode
eMB3RTUInit( u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity );

void
eMB3RTUStart( void );

u8      xMB3RTUReceiveFSM( void );

eMB3ErrorCode 
eMB3RTUReceive( u8 * pucRcvAddress, u8 ** pucFrame, u16 * pusLength );

eMB3ErrorCode
eMB3RTUSend( u8 ucSlaveAddress, const u8 * pucFrame, u16 usLength );

u8	xMB3RTUTransmitFSM( void );

u8      xMB3RTUTimerT35Expired( void );



eMB3Exception   MB3Error2Exception( eMB3ErrorCode eErrorCode );

eMB3Exception   eMB3FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen );


eMB3Exception   eMB3FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen );

#endif