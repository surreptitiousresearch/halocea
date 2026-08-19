/* get_number_of_updates_until_useful @0x8376DCC0 — how many more client updates must arrive before
 * `candidate_update_id` becomes the oldest still-useful one, given the player's queued remote actions.
 * Returns -1 if the queue is empty, 0 if the candidate is already at or behind the queue's oldest id, the
 * wrapped distance if the candidate id has wrapped around the 64-value update-id counter, or the plain
 * distance otherwise. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/simple_circular_queue.h"

extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);

int get_number_of_updates_until_useful(player_datum *player, int candidate_update_id)
{
    simple_circular_queue *queue = &player->___u26.client_update_data.___u0.remote_player.action_queue.queue;

    if ( simple_circular_queue_get_number_of_queued_entries(queue) <= 0 )
        return -1;

    int *oldest_entry;
    simple_circular_queue_peek(queue, (void **)&oldest_entry);

    int oldest_update_id = *oldest_entry;

    if ( candidate_update_id < oldest_update_id )
        return candidate_update_id - oldest_update_id + 64;
    if ( candidate_update_id <= oldest_update_id )
        return 0;

    return candidate_update_id - oldest_update_id;
}
