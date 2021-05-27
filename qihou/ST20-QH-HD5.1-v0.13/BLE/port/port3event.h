#include "stm32f10x.h"

#ifndef _PORT3_EVENT_H
#define _PORT3_EVENT_H


typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eMB3EventType;


/* ----------------------- Supporting functions -----------------------------*/
u8            xMB3PortEventInit( void );

u8            xMB3PortEventPost( eMB3EventType eEvent );

u8            xMB3PortEventGet(  /*@out@ */ eMB3EventType * eEvent );



#endif
