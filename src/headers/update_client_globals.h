#pragma once
/* _update_client_globals — the client-side prediction/replay queue: tracks which numbered updates have
 * been received and dequeued, the saved actions for the local players, and the ring of pending updates. */

#include <stdint.h>
#include "player_action_collection.h"
#include "update.h"
#include "data_array.h"

typedef struct _update_client_globals
{
    uint8_t          initialized;                     /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    int                      next_update_number_to_dequeue;   /* 0x04 */
    int                      latest_update_number_received;   /* 0x08 */
    player_action_collection saved_action_collection;         /* 0x0C */
    int                      client_ticks_to_apply_action_to; /* 0x4C */
    unsigned int             latched_control_flags;           /* 0x50 */
    int                      current_local_player;            /* 0x54 */
    data_array              *queues;                          /* 0x58 */
    update                   updates[128];                    /* 0x5C */
} _update_client_globals;

#ifdef __cplusplus
extern "C" {
#endif

extern _update_client_globals update_client_globals;

#ifdef __cplusplus
}
#endif
