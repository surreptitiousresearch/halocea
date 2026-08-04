#pragma once
/* _event_manager_globals @ event_manager_globals — UI/error event queue (event_manager.c) */

#include "event_record.h"

typedef struct _event_manager_globals
{
    unsigned char initialized;                       /* 0x00 */
    unsigned char suppressed;                        /* 0x01 */
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned int  milliseconds_at_last_event_queue;  /* 0x04 */
    unsigned int  milliseconds_at_last_queue_check;  /* 0x08 */
    event_record  event_queue[4][8];                 /* 0x0C */
} _event_manager_globals;                            /* 268 bytes */

extern _event_manager_globals event_manager_globals;
