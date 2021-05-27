

/* ----------------------- Modbus includes ----------------------------------*/
#include "port2event.h"


/* ----------------------- Variables ----------------------------------------*/
static eMB2EventType eQueuedEvent;
static u8     xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
u8
xMB2PortEventInit( void )
{
    xEventInQueue = 0;
    return 1;
}


u8
xMB2PortEventPost( eMB2EventType eEvent )
{
    xEventInQueue = 1;
    eQueuedEvent = eEvent;
    return 1;
}


u8
xMB2PortEventGet( eMB2EventType * eEvent )
{
    u8            xEventHappened = 0;

    if( xEventInQueue )
    {
        *eEvent = eQueuedEvent;
        xEventInQueue = 0;
        xEventHappened = 1;
    }
    return xEventHappened;
}
