/* action_queue_write @0x837AAFFC — enqueue a player action to replay. Builds a 44-byte queue entry
 * { client_update_id, repeat_count, ticks, player_action } — repeat_count and ticks both start at
 * ticks_to_apply_update_to (repeat_count is decremented as the action is consumed) — and pushes it onto the
 * queue's circular buffer.
 * DEVIATION: the decompiler bailed ("local variable allocation has failed") on the by-value 32-byte
 * player_action and the entry-build copy loop; reconstructed from disassembly. */

#include <stdint.h>
#include "headers/action_queue.h"

#include "headers/simple_circular_queue.h"
extern int simple_circular_queue_write(simple_circular_queue *queue, void *user_data);

uint8_t action_queue_write(action_queue *queue, player_action action, int client_update_id, int ticks_to_apply_update_to)
{
    action_entry entry;
    entry.client_update_id = client_update_id;
    entry.remaining_ticks_to_apply_action_to = ticks_to_apply_update_to;
    entry.total_ticks_to_apply_action_to = ticks_to_apply_update_to;
    entry.action = action;
    return simple_circular_queue_write(&queue->queue, &entry);
}
