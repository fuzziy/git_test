#include "mb2funcholding.h"
#include "my_modbus.h"



eMB2ErrorCode
eMB2RegHoldingCB( u8 * pucRegBuffer, u16 usAddress, u16 usNRegs, eMB2RegisterMode eMode )
{
	eMB2ErrorCode    eStatus = MB2_ENOERR;
	int             iRegIndex;
	int 		index;
	u16             nRegs = usNRegs;
	
	if((usAddress >= REG_HOLDING_START)&&\
           ((usAddress+usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		index = iRegIndex;
		switch(eMode)
		{                                       
                  case MB2_REG_READ://?? MB2_REG_READ = 0
                    while(usNRegs > 0)
                    {
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);            
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF); 
                        iRegIndex++;
                        usNRegs--;					
                    }                            
                    break;
                  case MB2_REG_WRITE://§Õ MB2_REG_WRITE = 0
                    while(usNRegs > 0)
                    {         
			usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                        usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                        iRegIndex++;
                        usNRegs--;
                    }	
                    //§Õ??????????????????¡À???????????????????????›¥??Flash??
                    Modbus_UpdateEngineValue(index,nRegs);
                    ///////////////////////////////
                    break;
		}
	}
	else//????
	{
		eStatus = MB2_ENOREG;
	}	
	
	return eStatus;
}



eMB2Exception
eMB2FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    u16          usRegCount;
    u8          *pucFrameCur;

    eMB2Exception    eStatus = MB2_EX_NONE;
    eMB2ErrorCode    eRegStatus;

    if( *usLen == ( MB2_PDU_FUNC_READ_SIZE + MB2_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB2_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB2_PDU_FUNC_READ_ADDR_OFF + 1] );
        usRegAddress++;

        usRegCount = ( u16 )( pucFrame[MB2_PDU_FUNC_READ_REGCNT_OFF] << 8 );
        usRegCount = ( u16 )( pucFrame[MB2_PDU_FUNC_READ_REGCNT_OFF + 1] );

        /* Check if the nuMB2er of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usRegCount >= 1 ) && ( usRegCount <= MB2_PDU_FUNC_READ_REGCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pucFrame[MB2_PDU_FUNC_OFF];
            *usLen = MB2_PDU_FUNC_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB2_FUNC_READ_HOLDING_REGISTER;
            *usLen += 1;

            /* Second byte in the response contain the nuMB2er of bytes. */
            *pucFrameCur++ = ( u8 ) ( usRegCount * 2 );
            *usLen += 1;

            /* Make callback to fill the buffer. */
            eRegStatus = eMB2RegHoldingCB( pucFrameCur, usRegAddress, usRegCount, MB2_REG_READ );
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB2_ENOERR )
            {
                eStatus = prveMB2Error2Exception( eRegStatus );
            }
            else
            {
                *usLen += usRegCount * 2;
            }
        }
        else
        {
            eStatus = MB2_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB2_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}



eMB2Exception
eMB2FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    eMB2Exception    eStatus = MB2_EX_NONE;
    eMB2ErrorCode    eRegStatus;

    if( *usLen == ( MB2_PDU_FUNC_WRITE_SIZE + MB2_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_ADDR_OFF + 1] );
        usRegAddress++;

        /* Make callback to update the value. */
        eRegStatus = eMB2RegHoldingCB( &pucFrame[MB2_PDU_FUNC_WRITE_VALUE_OFF],
                                      usRegAddress, 1, MB2_REG_WRITE );

        /* If an error occured convert it into a Modbus exception. */
        if( eRegStatus != MB2_ENOERR )
        {
            eStatus = prveMB2Error2Exception( eRegStatus );
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB2_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}


eMB2Exception
eMB2FuncWriteMultipleHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    u16          usRegCount;
    u8           ucRegByteCount;

    eMB2Exception    eStatus = MB2_EX_NONE;
    eMB2ErrorCode    eRegStatus;

    if( *usLen >= ( MB2_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB2_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );
        usRegAddress++;

        usRegCount = ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8 );
        usRegCount |= ( u16 )( pucFrame[MB2_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1] );

        ucRegByteCount = pucFrame[MB2_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

        if( ( usRegCount >= 1 ) &&
            ( usRegCount <= MB2_PDU_FUNC_WRITE_MUL_REGCNT_MAX ) &&
            ( ucRegByteCount == ( u8 ) ( 2 * usRegCount ) ) )
        {
            /* Make callback to update the register values. */
            eRegStatus =
                eMB2RegHoldingCB( &pucFrame[MB2_PDU_FUNC_WRITE_MUL_VALUES_OFF],
                                 usRegAddress, usRegCount, MB2_REG_WRITE );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB2_ENOERR )
            {
                eStatus = prveMB2Error2Exception( eRegStatus );
            }
            else
            {
                /* The response contains the function code, the starting
                 * address and the quantity of registers. We reuse the
                 * old values in the buffer because they are still valid.
                 */
                *usLen = MB2_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
            }
        }
        else
        {
            eStatus = MB2_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB2_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}



