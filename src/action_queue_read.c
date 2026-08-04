/* action_queue_read @0x837AB06C — pop the next buffered action from the queue. Peeks the head entry,
 * copies its 32-byte player_action payload (entry->action) into *action, and decrements the entry's repeat
 * count; when the count hits zero the entry is consumed (circular-queue read) and its client update id is
 * returned via completed_client_update_id (otherwise -1). The action is also cached as the queue's
 * last-valid action. Returns the peek/read status (1 = an action was produced).
 * DEVIATION: the decompiler's two 8x dword copy loops are 32-byte (sizeof player_action) block copies;
 * reproduced as memcpy. */

#include <stdint.h>
#include "headers/action_queue.h"

#include "headers/simple_circular_queue.h"
#include "headers/simple_circular_queue.h"
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);
extern int simple_circular_queue_read(simple_circular_queue *queue, void **user_data);
extern void *memcpy(void *dst, const void *src, unsigned int count);

/* return attested uint8_t: 2/4 callers clrlwi r3,24 (others ignore) */
uint8_t action_queue_read(action_queue *queue, player_action *action, int *completed_client_update_id)
{
    void *peeked[12];
    uint8_t result = simple_circular_queue_peek(&queue->queue, peeked);
    if ( result == 1 )
    {
        action_entry *entry = (action_entry *)peeked[0];
        memcpy(action, &entry->action, sizeof(player_action));
        *completed_client_update_id = -1;
        --entry->remaining_ticks_to_apply_action_to;
        if ( !entry->remaining_ticks_to_apply_action_to )
        {
            *completed_client_update_id = entry->client_update_id;
            /* simple_circular_queue_read extern says int; callers byte-normalize this fn's
             * return — width disagreement noted in the attestation ledger */
            result = (uint8_t)simple_circular_queue_read(&queue->queue, peeked);
        }
        queue->has_last_valid_action = 1;
        memcpy(&queue->last_valid_action, action, sizeof(player_action));
    }
    return result;
}
