#include "mb3rtu.h"
#include "mb3.h"
#include "os_cpu.h"
#include "calculate.h"
#include "rs485_3.h"
#include "my_modbus.h"
#include "port3timer.h"
#include "port3serial.h"
#include "port3event.h"



#define MB3_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB3_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB3_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB3_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB3_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/*****************???????????****************************/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMB3RcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMB3SndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMB3SndState eSndState;
static volatile eMB3RcvState eRcvState;

volatile u8  ucRTU3Buf[MB3_SER_PDU_SIZE_MAX];

static  u8 *pucSndBufferCur;
static  u16 usSndBufferCount;

static volatile u16 usRcvBufferPos;


eMB3ErrorCode
eMB3RTUInit( u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity )
{
    OS_CPU_SR cpu_sr=0;
    eMB3ErrorCode    eStatus = MB3_ENOERR;
    u32           usTimerT35_50us;


    ( void )ucSlaveAddress;
    OS_ENTER_CRITICAL(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMB3PortSerialInit( ucPort, ulBaudRate, 8, eParity ) != 1 )
    {
        eStatus = MB3_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        if( xMB3PortTimersInit( ( u16 ) usTimerT35_50us ) != 1 )
        {
            eStatus = MB3_EPORTERR;
        }
    }
    OS_EXIT_CRITICAL(  );

    return eStatus;
}


void
eMB3RTUStart( void )
{
    OS_CPU_SR cpu_sr=0;
	
    OS_ENTER_CRITICAL(  );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState = STATE_RX_INIT;
    vMB3PortSerialEnable( 1, 0 );
    vMB3PortTimersEnable(  );

    OS_EXIT_CRITICAL(  );
}




u8
xMB3RTUReceiveFSM( void )
{
    u8            xTaskNeedSwitch = 0;
    u8           ucByte;

  	
    /* Always read the character. */
    ( void )xMB3PortSerialGetByte( ( u8 * ) & ucByte );

    switch ( eRcvState )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMB3PortTimersEnable( );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMB3PortTimersEnable( );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        usRcvBufferPos = 0;
        ucRTU3Buf[usRcvBufferPos++] = ucByte;
        eRcvState = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vMB3PortTimersEnable( );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if( usRcvBufferPos < MB3_SER_PDU_SIZE_MAX )
        {
            ucRTU3Buf[usRcvBufferPos++] = ucByte;
        }
        else
        {
            eRcvState = STATE_RX_ERROR;
        }
        vMB3PortTimersEnable();
        break;
    }
#ifdef	DEBUG
	printf("xMB3RTUReceiveFSM(),ucByte=%d,eRcvState=%d\r\n",ucByte,eRcvState);	
#endif			
		
    return xTaskNeedSwitch;
}



eMB3ErrorCode
eMB3RTUReceive( u8 * pucRcvAddress, u8 ** pucFrame, u16 * pusLength )
{
    OS_CPU_SR cpu_sr=0;
    //u8            xFrameReceived = 0;
    eMB3ErrorCode    eStatus = MB3_ENOERR;

	
    OS_ENTER_CRITICAL(  );
   
    /* Length and CRC check */
    if( ( usRcvBufferPos >= MB3_SER_PDU_SIZE_MIN )
        && ( CRC16( ( u8 * ) ucRTU3Buf, usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucRTU3Buf[MB3_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( u16 )( usRcvBufferPos - MB3_SER_PDU_PDU_OFF - MB3_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( u8 * ) & ucRTU3Buf[MB3_SER_PDU_PDU_OFF];
       // xFrameReceived = 1;
    }
    else
    {
        eStatus = MB3_EIO;
    }

    OS_EXIT_CRITICAL(  );
    return eStatus;
}


eMB3ErrorCode
eMB3RTUSend( u8 ucSlaveAddress, const u8 * pucFrame, u16 usLength )
{
    OS_CPU_SR cpu_sr=0;
    eMB3ErrorCode    eStatus = MB3_ENOERR;
    u16         usCRC16;

	
    OS_ENTER_CRITICAL(  );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = ( u8 * ) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucSndBufferCur[MB3_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = CRC16( ( u8 * ) pucSndBufferCur, usSndBufferCount );
        ucRTU3Buf[usSndBufferCount++] = ( u8 )( usCRC16 >> 8 );
        ucRTU3Buf[usSndBufferCount++] = ( u8 )( usCRC16 & 0xFF );

        /* Activate the transmitter. */
        eSndState = STATE_TX_XMIT;
        //debug
         RS485_3rd_SendData(pucSndBufferCur,usSndBufferCount);
        xMB3PortEventPost( EV_FRAME_SENT );
        /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
        vMB3PortSerialEnable( 1, 0 );
        eSndState = STATE_TX_IDLE;
        //debug
        //vMB3PortSerialEnable( 0, 1 );
    }
    else
    {
        eStatus = MB3_EIO;
    }
    OS_EXIT_CRITICAL(  );
    return eStatus;
}


u8
xMB3RTUTransmitFSM( void )
{
    u8            xNeedPoll = 0;

    switch ( eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMB3PortSerialEnable( 1, 0 );
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if( usSndBufferCount != 0 )
        {
            xMB3PortSerialPutByte( ( u8 )*pucSndBufferCur );
            pucSndBufferCur++;  /* next byte in sendbuffer. */
            usSndBufferCount--;
        }
        else
        {
            xNeedPoll = xMB3PortEventPost( EV_FRAME_SENT );
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMB3PortSerialEnable( 1, 0 );
            eSndState = STATE_TX_IDLE;
        }
        break;
    }

    return xNeedPoll;
}




eMB3Exception
MB3Error2Exception( eMB3ErrorCode eErrorCode )
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


u8
xMB3RTUTimerT35Expired( void )
{
    u8            xNeedPoll = 0;
	

    switch ( eRcvState )
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        xNeedPoll = xMB3PortEventPost( EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        xNeedPoll = xMB3PortEventPost( EV_FRAME_RECEIVED );
        break;

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;
    }

    vMB3PortTimersDisable(  );
    eRcvState = STATE_RX_IDLE;

    return xNeedPoll;
}
