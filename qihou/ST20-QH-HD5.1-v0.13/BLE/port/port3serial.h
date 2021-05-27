#include "stm32f10x.h"

#ifndef _PORT3_SERIAL_H
#define _PORT3_SERIAL_H
	

/* ----------------------- Start implementation -----------------------------*/
void vMB3PortSerialEnable( u8 xRxEnable, u8 xTxEnable );

void vMB3PortClose( void );

u8 xMB3PortSerialInit( u8 ucPORT, u32 ulBaudRate, u8 ucDataBits, u8 eParity );


u8 xMB3PortSerialPutByte( u8 ucByte );

u8 xMB3PortSerialGetByte(u8 * pucByte );


void prvvUART3TxReadyISR(void);


void prvvUART3RxISR(void);



#endif
