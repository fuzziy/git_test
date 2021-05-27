

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "os_cpu.h"
#include "mb3.h"
#include "rs485_3.h"
#include "calculate.h"
#include "my_modbus.h"
#include "mb3rtu.h"
#include "port3.h"
#include "port3event.h"
#include "port3serial.h"
#include "port3timer.h"
#include "mb3funcholding.h"






static peMB3FrameSend peMB3FrameSendCur;
static pvMB3FrameStart pvMB3FrameStartCur;
static peMB3FrameReceive peMB3FrameReceiveCur;


/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 */
u8( *pxMB3FrameCBByteReceived ) ( void );
u8( *pxMB3FrameCBTransmitterEmpty ) ( void );
u8( *pxMB3PortCBTimerExpired ) ( void );

u8( *pxMB3FrameCBReceiveFSMCur ) ( void );
u8( *pxMB3FrameCBTransmitFSMCur ) ( void );
/* ----------------------- Static variables ---------------------------------*/

static u8    ucMB3Address;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMB3State = STATE_NOT_INITIALIZED;


typedef enum
{
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_WAIT_EOF           /*!< Wait for End of Frame. */
} eMB3RcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_START,             /*!< Starting transmission (':' sent). */
    STATE_TX_DATA,              /*!< Sending of data (Address, Data, LRC). */
    STATE_TX_END,               /*!< End of transmission. */
    STATE_TX_NOTIFY             /*!< Notify sender that the frame has been sent. */
} eMB3SndState;











eMB3ErrorCode
eMB3Init( u8 eMode, u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity )
{
    eMB3ErrorCode    eStatus = MB3_ENOERR;

    /* check preconditions */
    if( ( ucSlaveAddress == 0 ) ||
        ( ucSlaveAddress < 1 ) || ( ucSlaveAddress > 240 ) )
    {
        eStatus = MB3_EINVAL;
    }
    else
    {
        ucMB3Address = ucSlaveAddress;

       
        pvMB3FrameStartCur = eMB3RTUStart;
        peMB3FrameSendCur = eMB3RTUSend;
        peMB3FrameReceiveCur = eMB3RTUReceive;
        pxMB3FrameCBByteReceived = xMB3RTUReceiveFSM;
        pxMB3FrameCBTransmitterEmpty = xMB3RTUTransmitFSM;
        pxMB3PortCBTimerExpired = xMB3RTUTimerT35Expired;
        
        

        eStatus = eMB3RTUInit( ucMB3Address, ucPort, ulBaudRate, eParity );
      

        if( eStatus == MB3_ENOERR )
        {
            if( !xMB3PortEventInit(  ) )
            {
                /* port dependent event module initalization failed. */
                eStatus = MB3_EPORTERR;
            }
            else
            {
                eMB3State = STATE_DISABLED;
            }
        }
    }
    return eStatus;
}


eMB3ErrorCode
eMB3Enable( void )
{
    eMB3ErrorCode    eStatus = MB3_ENOERR;

    if( eMB3State == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        pvMB3FrameStartCur(  );
   
        eMB3State = STATE_ENABLED;
    }
    else
    {
        eStatus = MB3_EILLSTATE;
    }
    return eStatus;
}



eMB3ErrorCode eMB3Poll( void )
{
    static u8   *ucMB3Frame;
    static u8    ucRcvAddress;
    static u8    ucFunctionCode;
    static u16   usLength;
    static eMB3Exception eException;

    eMB3ErrorCode    eStatus = MB3_ENOERR;
    eMB3EventType    eEvent;

    /* Check if the protocol stack is ready. */
    if( eMB3State != STATE_ENABLED )
    {
        return MB3_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMB3PortEventGet( &eEvent ) == 1 )
    {
        switch ( eEvent )
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:
            eStatus = peMB3FrameReceiveCur( &ucRcvAddress, &ucMB3Frame, &usLength );//?????????????????????õ‘ucMB32Frame
            if( eStatus == MB3_ENOERR )
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if( ( ucRcvAddress == ucMB3Address ) || ( ucRcvAddress == 0 ) )
                {
                    ( void )xMB3PortEventPost( EV_EXECUTE );
                }
            }
            break;

        case EV_EXECUTE:
            ucFunctionCode = ucMB3Frame[MB3_PDU_FUNC_OFF];//?????????????????
            eException = MB3_EX_ILLEGAL_FUNCTION;
           
            /****************modbus…∏—°¥¶¿Ì*********************/
            switch( ucFunctionCode )
            {
              case 0x03:
                eException = eMB3FuncReadHoldingRegister( ucMB3Frame, &usLength );
                break;
              case 0x06:
                eException = eMB3FuncWriteHoldingRegister( ucMB3Frame, &usLength );
                break;
              case 0x10:
                eException = eMB3FuncWriteMultipleHoldingRegister( ucMB3Frame, &usLength );
                break;
            }

            /* If the request was not sent to the broadcast address we
             * return a reply. */
            if( ucRcvAddress != MB3_ADDRESS_BROADCAST )
            {
                if( eException != MB3_EX_NONE )
                {
                    /* An exception occured. Build an error frame. */
                    usLength = 0;
                    ucMB3Frame[usLength++] = ( u8 )( ucFunctionCode | MB3_FUNC_ERROR );
                    ucMB3Frame[usLength++] = eException;
                }
                eStatus = peMB3FrameSendCur( ucMB3Address, ucMB3Frame, usLength );
            }
            break;

        case EV_FRAME_SENT:
            break;
        }
    }
    return MB3_ENOERR;
}



eMB3Exception
prveMB3Error2Exception( eMB3ErrorCode eErrorCode )
{
    eMB3Exception    eStatus;

    switch ( eErrorCode )
    {
        case MB3_ENOERR:
            eStatus = MB3_EX_NONE;
            break;

        case MB3_ENOREG:
            eStatus = MB3_EX_ILLEGAL_DATA_ADDRESS;
            break;

        case MB3_ETIMEDOUT:
            eStatus = MB3_EX_SLAVE_BUSY;
            break;

        default:
            eStatus = MB3_EX_SLAVE_DEVICE_FAILURE;
            break;
    }

    return eStatus;
}

