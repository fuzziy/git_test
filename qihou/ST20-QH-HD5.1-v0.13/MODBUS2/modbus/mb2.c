

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "os_cpu.h"
#include "mb2.h"
#include "rs485_2.h"
#include "calculate.h"
#include "my_modbus.h"
#include "mb2rtu.h"
#include "port2.h"
#include "port2event.h"
#include "port2serial.h"
#include "port2timer.h"
#include "mb2funcholding.h"






static peMB2FrameSend peMB2FrameSendCur;
static pvMB2FrameStart pvMB2FrameStartCur;
static peMB2FrameReceive peMB2FrameReceiveCur;


/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 */
u8( *pxMB2FrameCBByteReceived ) ( void );
u8( *pxMB2FrameCBTransmitterEmpty ) ( void );
u8( *pxMB2PortCBTimerExpired ) ( void );

u8( *pxMB2FrameCBReceiveFSMCur ) ( void );
u8( *pxMB2FrameCBTransmitFSMCur ) ( void );
/* ----------------------- Static variables ---------------------------------*/

static u8    ucMB2Address;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMB2State = STATE_NOT_INITIALIZED;


typedef enum
{
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_WAIT_EOF           /*!< Wait for End of Frame. */
} eMB2RcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_START,             /*!< Starting transmission (':' sent). */
    STATE_TX_DATA,              /*!< Sending of data (Address, Data, LRC). */
    STATE_TX_END,               /*!< End of transmission. */
    STATE_TX_NOTIFY             /*!< Notify sender that the frame has been sent. */
} eMB2SndState;











eMB2ErrorCode
eMB2Init( u8 eMode, u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity )
{
    eMB2ErrorCode    eStatus = MB2_ENOERR;

    /* check preconditions */
    if( ( ucSlaveAddress == 0 ) ||
        ( ucSlaveAddress < 1 ) || ( ucSlaveAddress > 240 ) )
    {
        eStatus = MB2_EINVAL;
    }
    else
    {
        ucMB2Address = ucSlaveAddress;

       
        pvMB2FrameStartCur = eMB2RTUStart;
        peMB2FrameSendCur = eMB2RTUSend;
        peMB2FrameReceiveCur = eMB2RTUReceive;
        pxMB2FrameCBByteReceived = xMB2RTUReceiveFSM;
        pxMB2FrameCBTransmitterEmpty = xMB2RTUTransmitFSM;
        pxMB2PortCBTimerExpired = xMB2RTUTimerT35Expired;
        
        

        eStatus = eMB2RTUInit( ucMB2Address, ucPort, ulBaudRate, eParity );
      

        if( eStatus == MB2_ENOERR )
        {
            if( !xMB2PortEventInit(  ) )
            {
                /* port dependent event module initalization failed. */
                eStatus = MB2_EPORTERR;
            }
            else
            {
                eMB2State = STATE_DISABLED;
            }
        }
    }
    return eStatus;
}


eMB2ErrorCode
eMB2Enable( void )
{
    eMB2ErrorCode    eStatus = MB2_ENOERR;

    eMB2State = STATE_ENABLED;
   
    return eStatus;
}



eMB2ErrorCode eMB2Poll( void )
{
    static u8   *ucMB2Frame;
    static u8    ucRcvAddress;
    static u8    ucFunctionCode;
    static u16   usLength;
    static eMB2Exception eException;

    eMB2ErrorCode    eStatus = MB2_ENOERR;
    eMB2EventType    eEvent;

    /* Check if the protocol stack is ready. */
    if( eMB2State != STATE_ENABLED )
    {
        return MB2_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMB2PortEventGet( &eEvent ) == 1 )
    {  
        switch ( eEvent )
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:
            eStatus = peMB2FrameReceiveCur( &ucRcvAddress, &ucMB2Frame, &usLength );//?????????????????????õ‘ucMB22Frame
            
            if( eStatus == MB2_ENOERR )
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if( ( ucRcvAddress == ucMB2Address ) || ( ucRcvAddress == 0 ) )
                {
                    ( void )xMB2PortEventPost( EV_EXECUTE );
                }
            }
            break;

        case EV_EXECUTE:
            ucFunctionCode = ucMB2Frame[MB2_PDU_FUNC_OFF];//?????????????????
            eException = MB2_EX_ILLEGAL_FUNCTION;
            /****************modbus…∏—°¥¶¿Ì*********************/
            switch( ucFunctionCode )
            {
              case 0x03:
                eException = eMB2FuncReadHoldingRegister( ucMB2Frame, &usLength );
                break;
              case 0x06:
                eException = eMB2FuncWriteHoldingRegister( ucMB2Frame, &usLength );
                break;
              case 0x10:
                eException = eMB2FuncWriteMultipleHoldingRegister( ucMB2Frame, &usLength );
                break;
            }
            /* If the request was not sent to the broadcast address we
             * return a reply. */
            if( ucRcvAddress != MB2_ADDRESS_BROADCAST )
            {
                if( eException != MB2_EX_NONE )
                {
                    /* An exception occured. Build an error frame. */
                    usLength = 0;
                    ucMB2Frame[usLength++] = ( u8 )( ucFunctionCode | MB2_FUNC_ERROR );
                    ucMB2Frame[usLength++] = eException;
                }
                eStatus = peMB2FrameSendCur( ucMB2Address, ucMB2Frame, usLength );
            }
            break;

        case EV_FRAME_SENT:
            break;
        }
    }
    return MB2_ENOERR;
}



eMB2Exception
prveMB2Error2Exception( eMB2ErrorCode eErrorCode )
{
    eMB2Exception    eStatus;

    switch ( eErrorCode )
    {
        case MB2_ENOERR:
            eStatus = MB2_EX_NONE;
            break;

        case MB2_ENOREG:
            eStatus = MB2_EX_ILLEGAL_DATA_ADDRESS;
            break;

        case MB2_ETIMEDOUT:
            eStatus = MB2_EX_SLAVE_BUSY;
            break;

        default:
            eStatus = MB2_EX_SLAVE_DEVICE_FAILURE;
            break;
    }

    return eStatus;
}

