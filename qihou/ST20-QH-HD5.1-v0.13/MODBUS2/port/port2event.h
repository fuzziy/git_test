#include "stm32f10x.h"

#ifndef _PORT2_EVENT_H
#define _PORT2_EVENT_H


typedef enum
{
    EV_READY,                   /*!< Startup finished. */
    EV_FRAME_RECEIVED,          /*!< Frame received. */
    EV_EXECUTE,                 /*!< Execute function. */
    EV_FRAME_SENT               /*!< Frame sent. */
} eMB2EventType;


/* ----------------------- Supporting functions -----------------------------*/
u8            xMB2PortEventInit( void );

u8            xMB2PortEventPost( eMB2EventType eEvent );

u8            xMB2PortEventGet(  /*@out@ */ eMB2EventType * eEvent );



#endif
