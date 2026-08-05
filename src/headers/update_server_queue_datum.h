#pragma once
/* update_server_queue_datum — one per-machine element of update_server_globals.queues (100-byte
 * stride). Rebound to the DB type update_server_queue: the former opaque[92] tail is the machine's
 * current_action + action_queue; the former update_number field is the DB's next_update_number. */

#include <stdint.h>
#include "player_action.h"
#include "action_queue.h"

typedef struct update_server_queue
{
    int16_t       identifier;         /* 0x00 — data_array datum identifier */
    uint16_t pad;             /* 0x02 */
    int           next_update_number; /* 0x04 — next update number to send this machine */
    player_action current_action;     /* 0x08 */
    action_queue  queue;              /* 0x28 */
} update_server_queue;                /* 100 bytes */

typedef update_server_queue update_server_queue_datum;
