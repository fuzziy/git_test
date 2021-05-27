#include "stm32f10x.h"
#include "mb3.h"

#ifndef _MB3_FUNCHOLDING_H
#define _MB3_FUNCHOLDING_H

typedef enum
{
    MB3_REG_READ,                /*!< Read register values and pass to protocol stack. */
    MB3_REG_WRITE                /*!< Update register values. */
} eMB3RegisterMode;



eMB3Exception
eMB3FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen );


eMB3Exception
eMB3FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen );

eMB3Exception
eMB3FuncWriteMultipleHoldingRegister( u8 * pucFrame, u16 * usLen );








#endif