

/* ----------------------- Modbus includes ----------------------------------*/
#include "port3event.h"


/* ----------------------- Variables ----------------------------------------*/
static eMB3EventType eQueuedEvent;
static u8     xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
u8
xMB3PortEventInit( void )
{
    xEventInQueue = 0;
    return 1;
}


u8
xMB3PortEventPost( eMB3EventType eEvent )
{
    xEventInQueue = 1;
    eQueuedEvent = eEvent;
    return 1;
}


u8
xMB3PortEventGet( eMB3EventType * eEvent )
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
