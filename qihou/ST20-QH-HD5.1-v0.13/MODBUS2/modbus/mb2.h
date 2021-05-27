#include "stm32f10x.h"

#ifndef _MB2_H
#define _MB2_H
	


#define MB2_PDU_SIZE_MAX        253 /*!< Maximum size of a PDU. */
#define MB2_PDU_SIZE_MIN        1   /*!< Function Code */
#define MB2_PDU_FUNC_OFF        0   /*!< Offset of function code in PDU. */
#define MB2_PDU_DATA_OFF        1   /*!< Offset for response data in PDU. */


#define MB2_PDU_FUNC_READ_ADDR_OFF           ( MB2_PDU_DATA_OFF )
#define MB2_PDU_FUNC_READ_COILCNT_OFF        ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_READ_SIZE               ( 4 )
#define MB2_PDU_FUNC_READ_COILCNT_MAX        ( 0x07D0 )

#define MB2_PDU_FUNC_WRITE_ADDR_OFF          ( MB2_PDU_DATA_OFF )
#define MB2_PDU_FUNC_WRITE_VALUE_OFF         ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB2_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB2_PDU_DATA_OFF )
#define MB2_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB2_PDU_DATA_OFF + 4 )
#define MB2_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB2_PDU_DATA_OFF + 5 )
#define MB2_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
#define MB2_PDU_FUNC_WRITE_MUL_COILCNT_MAX   ( 0x07B0 )

#define MB2_PDU_FUNC_READ_ADDR_OFF           ( MB2_PDU_DATA_OFF )
#define MB2_PDU_FUNC_READ_DISCCNT_OFF        ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_READ_SIZE               ( 4 )
#define MB2_PDU_FUNC_READ_DISCCNT_MAX        ( 0x07D0 )

#define MB2_PDU_FUNC_READ_ADDR_OFF           ( MB2_PDU_DATA_OFF )
#define MB2_PDU_FUNC_READ_REGCNT_OFF         ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_READ_SIZE               ( 4 )
#define MB2_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )


//#define MB2_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB2_PDU_DATA_OFF + 0 )
#define MB2_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB2_PDU_DATA_OFF + 4 )
#define MB2_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB2_PDU_DATA_OFF + 5 )
#define MB2_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB2_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )

#define MB2_PDU_FUNC_READ_RSP_BYTECNT_OFF    ( MB2_PDU_DATA_OFF )


#define MB2_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB2_PDU_DATA_OFF + 0 )
#define MB2_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB2_PDU_DATA_OFF + 2 )
#define MB2_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB2_PDU_DATA_OFF + 4 )
#define MB2_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB2_PDU_DATA_OFF + 6 )
#define MB2_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB2_PDU_DATA_OFF + 8 )
#define MB2_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB2_PDU_DATA_OFF + 9 )
#define MB2_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )




#define MB2_ADDRESS_BROADCAST                   ( 0 )   /*! Modbus broadcast address. */
#define MB2_ADDRESS_MIN                         ( 1 )   /*! Smallest possible slave address. */
#define MB2_ADDRESS_MAX                         ( 247 ) /*! Biggest possible slave address. */
#define MB2_FUNC_NONE                           (  0 )
#define MB2_FUNC_READ_COILS                     (  1 )
#define MB2_FUNC_READ_DISCRETE_INPUTS           (  2 )
#define MB2_FUNC_WRITE_SINGLE_COIL              (  5 )
#define MB2_FUNC_WRITE_MULTIPLE_COILS           ( 15 )
#define MB2_FUNC_READ_HOLDING_REGISTER          (  3 )
#define MB2_FUNC_READ_INPUT_REGISTER            (  4 )
#define MB2_FUNC_WRITE_REGISTER                 (  6 )
#define MB2_FUNC_WRITE_MULTIPLE_REGISTERS       ( 16 )
#define MB2_FUNC_READWRITE_MULTIPLE_REGISTERS   ( 23 )
#define MB2_FUNC_DIAG_READ_EXCEPTION            (  7 )
#define MB2_FUNC_DIAG_DIAGNOSTIC                (  8 )
#define MB2_FUNC_DIAG_GET_COM_EVENT_CNT         ( 11 )
#define MB2_FUNC_DIAG_GET_COM_EVENT_LOG         ( 12 )
#define MB2_FUNC_OTHER_REPORT_SLAVEID           ( 17 )
#define MB2_FUNC_ERROR                          ( 128 )
#define MB_FUNC_HANDLERS_MAX                    ( 16 )


#define MB2_FUNC_HANDLERS_MAX                    ( 16 )
#define MB2_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )
#define MB2_FUNC_OTHER_REP_SLAVEID_ENABLED       (  0 )
#define MB2_FUNC_READ_INPUT_ENABLED              (  0 )
#define MB2_FUNC_READ_HOLDING_ENABLED            (  1 )
#define MB2_FUNC_WRITE_HOLDING_ENABLED           (  1 )
#define MB2_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
#define MB2_FUNC_READ_COILS_ENABLED              (  0 )
#define MB2_FUNC_WRITE_COIL_ENABLED              (  0 )
#define MB2_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  0 )
#define MB2_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  0 )
#define MB2_FUNC_READWRITE_HOLDING_ENABLED       (  1 )



    typedef enum
{
    MB2_EX_NONE = 0x00,
    MB2_EX_ILLEGAL_FUNCTION = 0x01,
    MB2_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MB2_EX_ILLEGAL_DATA_VALUE = 0x03,
    MB2_EX_SLAVE_DEVICE_FAILURE = 0x04,
    MB2_EX_ACKNOWLEDGE = 0x05,
    MB2_EX_SLAVE_BUSY = 0x06,
    MB2_EX_MEMORY_PARITY_ERROR = 0x08,
    MB2_EX_GATEWAY_PATH_FAILED = 0x0A,
    MB2_EX_GATEWAY_TGT_FAILED = 0x0B
} eMB2Exception;


typedef enum
{
    MB2_ENOERR,                  /*!< no error. */
    MB2_ENOREG,                  /*!< illegal register address. */
    MB2_EINVAL,                  /*!< illegal argument. */
    MB2_EPORTERR,                /*!< porting layer error. */
    MB2_ENORES,                  /*!< insufficient resources. */
    MB2_EIO,                     /*!< I/O error. */
    MB2_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB2_ETIMEDOUT                /*!< timeout error occurred. */
} eMB2ErrorCode;



typedef         eMB2Exception( *pxMB2FunctionHandler ) ( u8 * pucFrame, u16 * pusLength );

typedef struct
{
    u8           ucFunctionCode;
    pxMB2FunctionHandler pxHandler;
} xMB2FunctionHandler;




/* ----------------------- Prototypes  0-------------------------------------*/
typedef void    ( *pvMB2FrameStart ) ( void );

typedef void    ( *pvMB2FrameStop ) ( void );

typedef eMB2ErrorCode( *peMB2FrameReceive ) ( u8 * pucRcvAddress,
                                            u8 ** pucFrame,
                                            u16 * pusLength );

typedef eMB2ErrorCode( *peMB2FrameSend ) ( u8 slaveAddress,
                                         const u8 * pucFrame,
                                         u16 usLength );


eMB2Exception
prveMB2Error2Exception( eMB2ErrorCode eErrorCode );


eMB2ErrorCode
eMB2Init( u8 eMode, u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity );

eMB2ErrorCode   eMB2Enable( void );

eMB2ErrorCode eMB2Poll( void );

#endif
