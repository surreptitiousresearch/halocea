#pragma once
/* hs_thread — one running script thread (536 bytes; DB-verified). */

#include "hs_stack_frame.h"

typedef struct hs_thread
{
    __int16         identifier;         /* 0x00 */
    unsigned __int8 type;               /* 0x02 */
    unsigned __int8 flags;              /* 0x03 */
    int             script_index;       /* 0x04 */
    int             sleep_until;        /* 0x08 */
    int             latent_sleep_until; /* 0x0C */
    hs_stack_frame *stack;              /* 0x10 */
    int             result;             /* 0x14 */
    unsigned char   stack_data[512];    /* 0x18 */
} hs_thread;                            /* 536 bytes */
