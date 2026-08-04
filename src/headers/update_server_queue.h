#ifndef UPDATE_SERVER_QUEUE_H
#define UPDATE_SERVER_QUEUE_H

/* update_server_queue — one per-player entry in the update-server queue pool (100-byte datum).
 * Layout from the IDA DB type `update_server_queue` (types_members). */

#include "player_action.h"
#include "action_queue.h"

typedef struct update_server_queue
{
    __int16          identifier;         /* 0x00 */
    unsigned __int16 pad;                /* 0x02 */
    int              next_update_number; /* 0x04 */
    player_action    current_action;     /* 0x08 */
    action_queue     queue;              /* 0x28 */
} update_server_queue;                   /* 100 bytes */

#endif /* UPDATE_SERVER_QUEUE_H */
