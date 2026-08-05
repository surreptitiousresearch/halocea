#pragma once
/* _update_server_globals — the server-side numbered-update queue: the next update number to build, the queue
 * data array, and the ring of pending updates. */

#include <stdint.h>
#include "update.h"
#include "data_array.h"

typedef struct _update_server_globals
{
    uint8_t initialized;                 /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    int             next_update_number_to_build;  /* 0x04 */
    data_array     *queues;                       /* 0x08 */
    update          updates[32];                  /* 0x0C */
} _update_server_globals;

extern _update_server_globals update_server_globals;
