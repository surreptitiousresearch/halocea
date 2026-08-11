/* player_died @0x836A81B8 — bookkeeping when a player's unit dies. Detaches the player from its unit
 * (saving it as the dead-unit index), fires the local-player "player_died" telemetry/control reset,
 * and recomputes the all-players-dead flag. Then, depending on whether the player has quit and on the
 * network role, either runs the quit cleanup, notifies the server, or (on a client, for a non-local
 * player) clears the player's queued update history. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/players_globals.h"
#include "headers/simple_circular_queue.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"
#include "headers/player_update_history.h"

#include "headers/network_game_client.h"
extern void hcex_fire_plr_event(const char *eventName, int plr_idx);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern int16_t game_connection(void);
extern void update_server_player_died(uint16_t player_index);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_clear(struct player_update_history *history);
extern void player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(int player_index);
extern void simple_circular_queue_empty(simple_circular_queue *queue);

void player_died(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    int unit_index = player->unit_index;
    int16_t local_player_index = player->local_player_index;
    player->unit_index = -1;
    player->dead_unit_index = unit_index;
    if (local_player_index != -1)
    {
        hcex_fire_plr_event("player_died", local_player_index);
        player_control_new_unit(player->local_player_index, -1);
    }

    players_globals->all_dead = 1;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for (player_datum *other = data_iterator_next(&iterator); other;
         other = data_iterator_next(&iterator))
    {
        if (other->unit_index != -1)
            players_globals->all_dead = 0;
    }

    if (player->quit_out_of_game == 1)
    {
        player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(player_index);
    }
    else
    {
        if (game_connection() == _game_connection_network_server)
            update_server_player_died(player_index);
        if (player->local_player_index == -1)   /* not a local player */
        {
            if (game_connection() == _game_connection_network_client)   /* client */
            {
                client_remote_player_data *remote = &player->___u26.client_update_data.___u0.remote_player;
                simple_circular_queue_empty(&remote->action_queue.queue);
                simple_circular_queue_empty(&remote->position_queue.queue);
                simple_circular_queue_empty(&remote->vehicle_update_queue.queue);
                remote->action_queue.last_valid_action.control_flags = 0;
                remote->action_queue.last_valid_action.throttle.n[0] = 0.0f;
                remote->action_queue.last_valid_action.throttle.n[1] = 0.0f;
            }
        }
        else if (global_network_game_client_get())
        {
            player_update_history_clear(nullptr);
        }
    }
}
