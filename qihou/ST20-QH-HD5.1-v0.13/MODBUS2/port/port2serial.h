#include "stm32f10x.h"

#ifndef _PORT2_SERIAL_H
#define _PORT2_SERIAL_H
	

/* ----------------------- Start implementation -----------------------------*/
void vMB2PortSerialEnable( u8 xRxEnable, u8 xTxEnable );

void vMB2PortClose( void );

u8 xMB2PortSerialInit( u8 ucPORT, u32 ulBaudRate, u8 ucDataBits, u8 eParity );


u8 xMB2PortSerialPutByte( u8 ucByte );

u8 xMB2PortSerialGetByte(u8 * pucByte );


void prvvUART2TxReadyISR(void);


void prvvUART2RxISR(void);



#endif
