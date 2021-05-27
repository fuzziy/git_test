#include "mb3funcholding.h"
#include "my_modbus.h"



eMB3ErrorCode
eMB3RegHoldingCB( u8 * pucRegBuffer, u16 usAddress, u16 usNRegs, eMB3RegisterMode eMode )
{
	eMB3ErrorCode    eStatus = MB3_ENOERR;
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
                  case MB3_REG_READ://?? MB3_REG_READ = 0
                    while(usNRegs > 0)
                    {
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] >> 8);            
			*pucRegBuffer++ = (u8)(usRegHoldingBuf[iRegIndex] & 0xFF); 
                        iRegIndex++;
                        usNRegs--;					
                    }                            
                    break;
                  case MB3_REG_WRITE://§Õ MB3_REG_WRITE = 0
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
		eStatus = MB3_ENOREG;
	}	
	
	return eStatus;
}



eMB3Exception
eMB3FuncReadHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    u16          usRegCount;
    u8          *pucFrameCur;

    eMB3Exception    eStatus = MB3_EX_NONE;
    eMB3ErrorCode    eRegStatus;

    if( *usLen == ( MB3_PDU_FUNC_READ_SIZE + MB3_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB3_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB3_PDU_FUNC_READ_ADDR_OFF + 1] );
        usRegAddress++;

        usRegCount = ( u16 )( pucFrame[MB3_PDU_FUNC_READ_REGCNT_OFF] << 8 );
        usRegCount = ( u16 )( pucFrame[MB3_PDU_FUNC_READ_REGCNT_OFF + 1] );

        /* Check if the nuMB3er of registers to read is valid. If not
         * return Modbus illegal data value exception. 
         */
        if( ( usRegCount >= 1 ) && ( usRegCount <= MB3_PDU_FUNC_READ_REGCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pucFrame[MB3_PDU_FUNC_OFF];
            *usLen = MB3_PDU_FUNC_OFF;

            /* First byte contains the function code. */
            *pucFrameCur++ = MB3_FUNC_READ_HOLDING_REGISTER;
            *usLen += 1;

            /* Second byte in the response contain the nuMB3er of bytes. */
            *pucFrameCur++ = ( u8 ) ( usRegCount * 2 );
            *usLen += 1;

            /* Make callback to fill the buffer. */
            eRegStatus = eMB3RegHoldingCB( pucFrameCur, usRegAddress, usRegCount, MB3_REG_READ );
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB3_ENOERR )
            {
                eStatus = prveMB3Error2Exception( eRegStatus );
            }
            else
            {
                *usLen += usRegCount * 2;
            }
        }
        else
        {
            eStatus = MB3_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB3_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}



eMB3Exception
eMB3FuncWriteHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    eMB3Exception    eStatus = MB3_EX_NONE;
    eMB3ErrorCode    eRegStatus;

    if( *usLen == ( MB3_PDU_FUNC_WRITE_SIZE + MB3_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_ADDR_OFF + 1] );
        usRegAddress++;

        /* Make callback to update the value. */
        eRegStatus = eMB3RegHoldingCB( &pucFrame[MB3_PDU_FUNC_WRITE_VALUE_OFF],
                                      usRegAddress, 1, MB3_REG_WRITE );

        /* If an error occured convert it into a Modbus exception. */
        if( eRegStatus != MB3_ENOERR )
        {
            eStatus = prveMB3Error2Exception( eRegStatus );
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB3_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}


eMB3Exception
eMB3FuncWriteMultipleHoldingRegister( u8 * pucFrame, u16 * usLen )
{
    u16          usRegAddress;
    u16          usRegCount;
    u8           ucRegByteCount;

    eMB3Exception    eStatus = MB3_EX_NONE;
    eMB3ErrorCode    eRegStatus;

    if( *usLen >= ( MB3_PDU_FUNC_WRITE_MUL_SIZE_MIN + MB3_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_MUL_ADDR_OFF] << 8 );
        usRegAddress |= ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_MUL_ADDR_OFF + 1] );
        usRegAddress++;

        usRegCount = ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_MUL_REGCNT_OFF] << 8 );
        usRegCount |= ( u16 )( pucFrame[MB3_PDU_FUNC_WRITE_MUL_REGCNT_OFF + 1] );

        ucRegByteCount = pucFrame[MB3_PDU_FUNC_WRITE_MUL_BYTECNT_OFF];

        if( ( usRegCount >= 1 ) &&
            ( usRegCount <= MB3_PDU_FUNC_WRITE_MUL_REGCNT_MAX ) &&
            ( ucRegByteCount == ( u8 ) ( 2 * usRegCount ) ) )
        {
            /* Make callback to update the register values. */
            eRegStatus =
                eMB3RegHoldingCB( &pucFrame[MB3_PDU_FUNC_WRITE_MUL_VALUES_OFF],
                                 usRegAddress, usRegCount, MB3_REG_WRITE );

            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB3_ENOERR )
            {
                eStatus = prveMB3Error2Exception( eRegStatus );
            }
            else
            {
                /* The response contains the function code, the starting
                 * address and the quantity of registers. We reuse the
                 * old values in the buffer because they are still valid.
                 */
                *usLen = MB3_PDU_FUNC_WRITE_MUL_BYTECNT_OFF;
            }
        }
        else
        {
            eStatus = MB3_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = MB3_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}



