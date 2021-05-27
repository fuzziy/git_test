#include "stm32f10x.h"
#include "mb2.h"

#ifndef _MB2_FUNCHOLDING_H
#define _MB2_FUNCHOLDING_H

typedef enum
{
    MB2_REG_READ,                /*!< Read register values and pass to protocol stack. */
    MB2_REG_WRITE                /*!< Update register values. */
} eMB2RegisterMode;



eMB2Exception
eMB2FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen );


eMB2Exception
eMB2FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen );

eMB2Exception
eMB2FuncWriteMultipleHoldingRegister( u8 * pucFrame, u16 * usLen );








#endif