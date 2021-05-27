#include "stm32f10x.h"

#ifndef _MB3_H
#define _MB3_H
	


#define MB3_PDU_SIZE_MAX        253 /*!< Maximum size of a PDU. */
#define MB3_PDU_SIZE_MIN        1   /*!< Function Code */
#define MB3_PDU_FUNC_OFF        0   /*!< Offset of function code in PDU. */
#define MB3_PDU_DATA_OFF        1   /*!< Offset for response data in PDU. */


#define MB3_PDU_FUNC_READ_ADDR_OFF           ( MB3_PDU_DATA_OFF )
#define MB3_PDU_FUNC_READ_COILCNT_OFF        ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_READ_SIZE               ( 4 )
#define MB3_PDU_FUNC_READ_COILCNT_MAX        ( 0x07D0 )

#define MB3_PDU_FUNC_WRITE_ADDR_OFF          ( MB3_PDU_DATA_OFF )
#define MB3_PDU_FUNC_WRITE_VALUE_OFF         ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_WRITE_SIZE              ( 4 )

#define MB3_PDU_FUNC_WRITE_MUL_ADDR_OFF      ( MB3_PDU_DATA_OFF )
#define MB3_PDU_FUNC_WRITE_MUL_COILCNT_OFF   ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_WRITE_MUL_BYTECNT_OFF   ( MB3_PDU_DATA_OFF + 4 )
#define MB3_PDU_FUNC_WRITE_MUL_VALUES_OFF    ( MB3_PDU_DATA_OFF + 5 )
#define MB3_PDU_FUNC_WRITE_MUL_SIZE_MIN      ( 5 )
#define MB3_PDU_FUNC_WRITE_MUL_COILCNT_MAX   ( 0x07B0 )

#define MB3_PDU_FUNC_READ_ADDR_OFF           ( MB3_PDU_DATA_OFF )
#define MB3_PDU_FUNC_READ_DISCCNT_OFF        ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_READ_SIZE               ( 4 )
#define MB3_PDU_FUNC_READ_DISCCNT_MAX        ( 0x07D0 )

#define MB3_PDU_FUNC_READ_ADDR_OFF           ( MB3_PDU_DATA_OFF )
#define MB3_PDU_FUNC_READ_REGCNT_OFF         ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_READ_SIZE               ( 4 )
#define MB3_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )


//#define MB3_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB3_PDU_DATA_OFF + 0 )
#define MB3_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_WRITE_MUL_BYTECNT_OFF       ( MB3_PDU_DATA_OFF + 4 )
#define MB3_PDU_FUNC_WRITE_MUL_VALUES_OFF        ( MB3_PDU_DATA_OFF + 5 )
#define MB3_PDU_FUNC_WRITE_MUL_SIZE_MIN          ( 5 )
#define MB3_PDU_FUNC_WRITE_MUL_REGCNT_MAX        ( 0x0078 )

#define MB3_PDU_FUNC_READ_RSP_BYTECNT_OFF    ( MB3_PDU_DATA_OFF )


#define MB3_PDU_FUNC_READWRITE_READ_ADDR_OFF     ( MB3_PDU_DATA_OFF + 0 )
#define MB3_PDU_FUNC_READWRITE_READ_REGCNT_OFF   ( MB3_PDU_DATA_OFF + 2 )
#define MB3_PDU_FUNC_READWRITE_WRITE_ADDR_OFF    ( MB3_PDU_DATA_OFF + 4 )
#define MB3_PDU_FUNC_READWRITE_WRITE_REGCNT_OFF  ( MB3_PDU_DATA_OFF + 6 )
#define MB3_PDU_FUNC_READWRITE_BYTECNT_OFF       ( MB3_PDU_DATA_OFF + 8 )
#define MB3_PDU_FUNC_READWRITE_WRITE_VALUES_OFF  ( MB3_PDU_DATA_OFF + 9 )
#define MB3_PDU_FUNC_READWRITE_SIZE_MIN          ( 9 )




#define MB3_ADDRESS_BROADCAST                   ( 0 )   /*! Modbus broadcast address. */
#define MB3_ADDRESS_MIN                         ( 1 )   /*! Smallest possible slave address. */
#define MB3_ADDRESS_MAX                         ( 247 ) /*! Biggest possible slave address. */
#define MB3_FUNC_NONE                           (  0 )
#define MB3_FUNC_READ_COILS                     (  1 )
#define MB3_FUNC_READ_DISCRETE_INPUTS           (  2 )
#define MB3_FUNC_WRITE_SINGLE_COIL              (  5 )
#define MB3_FUNC_WRITE_MULTIPLE_COILS           ( 15 )
#define MB3_FUNC_READ_HOLDING_REGISTER          (  3 )
#define MB3_FUNC_READ_INPUT_REGISTER            (  4 )
#define MB3_FUNC_WRITE_REGISTER                 (  6 )
#define MB3_FUNC_WRITE_MULTIPLE_REGISTERS       ( 16 )
#define MB3_FUNC_READWRITE_MULTIPLE_REGISTERS   ( 23 )
#define MB3_FUNC_DIAG_READ_EXCEPTION            (  7 )
#define MB3_FUNC_DIAG_DIAGNOSTIC                (  8 )
#define MB3_FUNC_DIAG_GET_COM_EVENT_CNT         ( 11 )
#define MB3_FUNC_DIAG_GET_COM_EVENT_LOG         ( 12 )
#define MB3_FUNC_OTHER_REPORT_SLAVEID           ( 17 )
#define MB3_FUNC_ERROR                          ( 128 )
#define MB_FUNC_HANDLERS_MAX                    ( 16 )


#define MB3_FUNC_HANDLERS_MAX                    ( 16 )
#define MB3_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )
#define MB3_FUNC_OTHER_REP_SLAVEID_ENABLED       (  0 )
#define MB3_FUNC_READ_INPUT_ENABLED              (  0 )
#define MB3_FUNC_READ_HOLDING_ENABLED            (  1 )
#define MB3_FUNC_WRITE_HOLDING_ENABLED           (  1 )
#define MB3_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
#define MB3_FUNC_READ_COILS_ENABLED              (  0 )
#define MB3_FUNC_WRITE_COIL_ENABLED              (  0 )
#define MB3_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  0 )
#define MB3_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  0 )
#define MB3_FUNC_READWRITE_HOLDING_ENABLED       (  1 )



    typedef enum
{
    MB3_EX_NONE = 0x00,
    MB3_EX_ILLEGAL_FUNCTION = 0x01,
    MB3_EX_ILLEGAL_DATA_ADDRESS = 0x02,
    MB3_EX_ILLEGAL_DATA_VALUE = 0x03,
    MB3_EX_SLAVE_DEVICE_FAILURE = 0x04,
    MB3_EX_ACKNOWLEDGE = 0x05,
    MB3_EX_SLAVE_BUSY = 0x06,
    MB3_EX_MEMORY_PARITY_ERROR = 0x08,
    MB3_EX_GATEWAY_PATH_FAILED = 0x0A,
    MB3_EX_GATEWAY_TGT_FAILED = 0x0B
} eMB3Exception;


typedef enum
{
    MB3_ENOERR,                  /*!< no error. */
    MB3_ENOREG,                  /*!< illegal register address. */
    MB3_EINVAL,                  /*!< illegal argument. */
    MB3_EPORTERR,                /*!< porting layer error. */
    MB3_ENORES,                  /*!< insufficient resources. */
    MB3_EIO,                     /*!< I/O error. */
    MB3_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB3_ETIMEDOUT                /*!< timeout error occurred. */
} eMB3ErrorCode;



typedef         eMB3Exception( *pxMB3FunctionHandler ) ( u8 * pucFrame, u16 * pusLength );

typedef struct
{
    u8           ucFunctionCode;
    pxMB3FunctionHandler pxHandler;
} xMB3FunctionHandler;




/* ----------------------- Prototypes  0-------------------------------------*/
typedef void    ( *pvMB3FrameStart ) ( void );

typedef void    ( *pvMB3FrameStop ) ( void );

typedef eMB3ErrorCode( *peMB3FrameReceive ) ( u8 * pucRcvAddress,
                                            u8 ** pucFrame,
                                            u16 * pusLength );

typedef eMB3ErrorCode( *peMB3FrameSend ) ( u8 slaveAddress,
                                         const u8 * pucFrame,
                                         u16 usLength );


eMB3Exception
prveMB3Error2Exception( eMB3ErrorCode eErrorCode );


eMB3ErrorCode
eMB3Init( u8 eMode, u8 ucSlaveAddress, u8 ucPort, u32 ulBaudRate, u8 eParity );

eMB3ErrorCode   eMB3Enable( void );

eMB3ErrorCode eMB3Poll( void );

#endif
