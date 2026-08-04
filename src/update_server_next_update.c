/* update_server_next_update @0x836F6AF8 — build the next outgoing numbered server update by draining one
 * action from each player's action queue. For each player: if a fresh action is available it is cached in
 * the player record (at +8) and appended to the update with its completed-client-update id; otherwise the
 * player's last cached action is re-sent with a completed id of -1. The finished update is handed to the
 * client-side handler (the server is also a local client). */

#include <stdint.h>
#include "headers/update_server_globals.h"
#include "headers/data_iterator.h"
#include "headers/player_action.h"
#include "headers/action_queue.h"
#include "headers/update_server_queue.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
#include "headers/action_queue.h"
#include "headers/game_update.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern uint8_t action_queue_read(action_queue *queue, player_action *action, int *completed_client_update_id);
extern void update_client_handle_server_update(game_update *new_update, int update_number);

extern update *update_server_get_update(int update_number);
void update_server_next_update(void)
{
    int update_number = update_server_globals.next_update_number_to_build++;

    /* DEVIATION: inlined copy of update_server_get_update@0x836F61F0 collapsed to a call; caller's pre-increment update_number makes both of the donor's guard branches unreachable here (always resolves to the in-range fetch), matching source pattern. */
    update *current = update_server_get_update(update_number);

    current->update_number = update_number;
    current->data.number_of_actions = 0;

    data_iterator iterator;
    data_iterator_new(&iterator, update_server_globals.queues);

    for (update_server_queue *player_record = data_iterator_next(&iterator);
         player_record;
         player_record = data_iterator_next(&iterator))
    {
        player_action action;
        int completed_client_update_id[4];
        player_action *cached_action = &player_record->current_action;

        if (action_queue_read(&player_record->queue, &action, completed_client_update_id) == 1)
        {
            *cached_action = action;
            current->data.actions[current->data.number_of_actions] = action;
            current->data.completed_client_update_id[current->data.number_of_actions++] = completed_client_update_id[0];
        }
        else
        {
            current->data.actions[current->data.number_of_actions] = *cached_action;
            current->data.completed_client_update_id[current->data.number_of_actions++] = -1;
        }
    }

    update_client_handle_server_update(&current->data, update_number);
}
