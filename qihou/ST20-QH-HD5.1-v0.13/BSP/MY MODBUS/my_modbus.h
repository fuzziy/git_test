#ifndef __MY_MODBUS_H
#define __MY_MODBUS_H
#include "stm32f10x.h"

#include "qihou_modbus.h"

#ifdef	MY_MODBUS_GLOBALS
#define MODBUS_EXT
#else
#define MODBUS_EXT	extern
#endif

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 						1
#define REG_INPUT_NREGS 						10
#define REG_HOLDING_START 						1



MODBUS_EXT	u16   usRegInputStart;
MODBUS_EXT	u16   usRegInputBuf[REG_INPUT_NREGS];
MODBUS_EXT	u16   usRegHoldingStart;
MODBUS_EXT	u16   usRegHoldingBuf[REG_HOLDING_NREGS];


void Modbus_UpdateRegInput(void);
void Modbus_UpdateRegHolding(void);
void Modbus_UpdateEngineValue( int index, u16 nRegs );


#endif
