#include "mb2rtu.h"
#include "mb2.h"
#include "os_cpu.h"
#include "calculate.h"
#include "rs485_2.h"
#include "my_modbus.h"
#include "port2timer.h"
#include "port2serial.h"
#include "port2event.h"



#define MB2_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB2_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB2_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB2_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB2_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/*****************???????????****************************/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMB2RcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMB2SndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMB2SndState eSndState;
static volatile eMB2RcvState eRcvState;

volatile u8  ucRTU2Buf[MB2_SER_PDU_SIZE_MAX];

static  u8 *pucSndBufferCur;
static  u16 usSndBufferCount;

static volatile u16 usRcvBufferPos;

u8 Flag_USART2_RX = 0;

/*****************************************************************************
*               串口2的初始化、DMA、中断服务函数
*
*
*
*****************************************************************************/
void RS485_2nd_DMAConfig(void)
{
	DMA_InitTypeDef  DMA_InitStructure;                     //定义DMA配置参数结构体对象
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);       //使能DMA时钟
	
	
	 DMA_DeInit(DMA1_Channel6);
	 DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART2->DR);
	 DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ucRTU2Buf;
	 DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	 DMA_InitStructure.DMA_BufferSize = MB2_SER_PDU_SIZE_MAX;
	 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	 DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	 DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	 DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	 DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	 DMA_Init(DMA1_Channel6,&DMA_InitStructure);
	 DMA_Cmd(DMA1_Channel6,ENABLE);
}



void RS485_2nd_Init(u32 BaudRate, u8 DataBits, u8 Parity)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//使能USART2，GPIOA时钟以及复用功能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	

   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = BaudRate;
	if(Parity>2)
		Parity = 0;
	switch(Parity)
	{
		case 0:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;
		case 1:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
		case 2:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
	}
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口
	//
        USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); 
        
	RS485_2nd_DMAConfig();
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  // enable Usart2's receive DMA
	USART_Cmd(USART2, ENABLE);                    //使能串口 
}





void USART2_IRQHandler(void)                	//串口1中断服务程序
{
  u16 temp;
  if(USART_GetITStatus(USART2, USART_FLAG_PE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_PE);
  }
  
     if(USART_GetITStatus(USART2, USART_FLAG_NE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_NE);
  }
  
    if(USART_GetITStatus(USART2, USART_FLAG_FE) != RESET)  
  {
    USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_FE);
  }
  
  if(USART_GetITStatus(USART2, USART_FLAG_ORE) != RESET)
    {
      USART_ReceiveData(USART2);
      USART_ClearFlag(USART2, USART_FLAG_ORE);
    }
    
  if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {	
      USART_ClearFlag(USART2, USART_IT_RXNE);
      USART_ClearITPendingBit(USART2, USART_IT_RXNE);
      //prvvUART2RxISR();	
  }

  if(USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  {
	prvvUART2TxReadyISR();
	USART_ClearITPendingBit(USART2, USART_IT_TXE);
  }

  if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
  {
      temp = USART2->SR;
      temp = USART2->DR;
      DMA_Cmd(DMA1_Channel6,DISABLE);    //DMA接收数据完成后，对应的数据总线空闲，则进入到USART2中断中，在该中断中，先关掉DMA，然后做简单的处理，如计算接收数据个数                                                      
      usRcvBufferPos = MB2_SER_PDU_SIZE_MAX - DMA_GetCurrDataCounter(DMA1_Channel6);        //计算接收数据个数
	  
      //很重要
      xMB2PortEventPost( EV_FRAME_RECEIVED );			
      eRcvState = STATE_RX_IDLE;                //接收完成，表示现在接收为空闲状态,必须要有，因为后续需要判断该状态
      //此处可尝试在发送完成后，再进行激活DMA，否则，可能会出现还没来得及返回数据，新的接收已经完成，这个还需要测试
      DMA_SetCurrDataCounter(DMA1_Channel6,MB2_SER_PDU_SIZE_MAX);      //重新给DMA设定传输个数
      DMA_Cmd(DMA1_Channel6,ENABLE);                  //启动新一轮的DMA传送	
  }

} 






eMB2ErrorCode
eMB2RTUInit( u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity )
{
    OS_CPU_SR cpu_sr=0;
    eMB2ErrorCode    eStatus = MB2_ENOERR;
    

    ( void )ucSlaveAddress;
    OS_ENTER_CRITICAL(  );

    /* Modbus RTU uses 8 Databits. */
    if( xMB2PortSerialInit( ucPort, ulBaudRate, 8, eParity ) != 1 )
    {
        eStatus = MB2_EPORTERR;
    }
    else
    {
        
            eStatus = MB2_EPORTERR;
       
    }
    OS_EXIT_CRITICAL(  );

    return eStatus;
}


void
eMB2RTUStart( void )
{
    OS_CPU_SR cpu_sr=0;
	
    OS_ENTER_CRITICAL(  );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    

    OS_EXIT_CRITICAL(  );
}




u8
xMB2RTUReceiveFSM( void )
{
    u8            xTaskNeedSwitch = 0;
    u8           ucByte;

  	
    /* Always read the character. */
    ( void )xMB2PortSerialGetByte( ( u8 * ) & ucByte );

    switch ( eRcvState )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMB2PortTimersEnable( );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMB2PortTimersEnable( );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        usRcvBufferPos = 0;
        ucRTU2Buf[usRcvBufferPos++] = ucByte;
        eRcvState = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vMB2PortTimersEnable( );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if( usRcvBufferPos < MB2_SER_PDU_SIZE_MAX )
        {
            ucRTU2Buf[usRcvBufferPos++] = ucByte;
        }
        else
        {
            eRcvState = STATE_RX_ERROR;
        }
        vMB2PortTimersEnable();
        break;
    }
#ifdef	DEBUG
	printf("xMB2RTUReceiveFSM(),ucByte=%d,eRcvState=%d\r\n",ucByte,eRcvState);	
#endif			
		
    return xTaskNeedSwitch;
}



eMB2ErrorCode
eMB2RTUReceive( u8 * pucRcvAddress, u8 ** pucFrame, u16 * pusLength )
{
    OS_CPU_SR cpu_sr=0;
    //u8            xFrameReceived = 0;
    eMB2ErrorCode    eStatus = MB2_ENOERR;

	
    OS_ENTER_CRITICAL(  );
    /* Length and CRC check */
    if( ( usRcvBufferPos >= MB2_SER_PDU_SIZE_MIN )
        && ( CRC16( ( u8 * ) ucRTU2Buf, usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucRTU2Buf[MB2_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( u16 )( usRcvBufferPos - MB2_SER_PDU_PDU_OFF - MB2_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( u8 * ) & ucRTU2Buf[MB2_SER_PDU_PDU_OFF];
       // xFrameReceived = 1;  
    }
    else
    {
        eStatus = MB2_EIO;
    }

    OS_EXIT_CRITICAL(  );
    return eStatus;
}


eMB2ErrorCode
eMB2RTUSend( u8 ucSlaveAddress, const u8 * pucFrame, u16 usLength )
{
    OS_CPU_SR cpu_sr=0;
    eMB2ErrorCode    eStatus = MB2_ENOERR;
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
        pucSndBufferCur[MB2_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = CRC16( ( u8 * ) pucSndBufferCur, usSndBufferCount );
        ucRTU2Buf[usSndBufferCount++] = ( u8 )( usCRC16 >> 8 );
        ucRTU2Buf[usSndBufferCount++] = ( u8 )( usCRC16 & 0xFF );

        /* Activate the transmitter. */
       
        //debug
         RS485_2nd_SendData(pucSndBufferCur,usSndBufferCount);
        xMB2PortEventPost( EV_FRAME_SENT );
        
        eRcvState = STATE_RX_RCV;               //此处不是表示正在接收，而是表示进入等待接收状态，这是在freemodbus的基础上演变的。
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
          
            eSndState = STATE_TX_IDLE;
        //debug
        //vMB2PortSerialEnable( 0, 1 );
    }
    else
    {
        eStatus = MB2_EIO;
    }
    OS_EXIT_CRITICAL(  );
    return eStatus;
}


u8
xMB2RTUTransmitFSM( void )
{
    u8            xNeedPoll = 0;

    switch ( eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMB2PortSerialEnable( 1, 0 );
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if( usSndBufferCount != 0 )
        {
            xMB2PortSerialPutByte( ( u8 )*pucSndBufferCur );
            pucSndBufferCur++;  /* next byte in sendbuffer. */
            usSndBufferCount--;
        }
        else
        {
            xNeedPoll = xMB2PortEventPost( EV_FRAME_SENT );
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMB2PortSerialEnable( 1, 0 );
            eSndState = STATE_TX_IDLE;
        }
        break;
    }

    return xNeedPoll;
}




eMB2Exception
MB2Error2Exception( eMB2ErrorCode eErrorCode )
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


u8
xMB2RTUTimerT35Expired( void )
{
    

    return 1;
}
