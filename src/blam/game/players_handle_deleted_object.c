/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* players_handle_deleted_object @0x836AA590 — when a unit-class object (biped or vehicle) is deleted, detach
 * any player riding it. Only runs for object types 0/1 (the (1<<type)&3 gate). For each player whose unit is
 * the deleted object, it clears the player's unit index (stashing the old one), fires the "player_died" script
 * event and requests a new controlled unit for local players, then recomputes the all-players-dead flag. A
 * player that has quit is cleaned up; otherwise the server records the death, and depending on connection state
 * the client either empties the player's prediction/update queues (when it has no current unit) or clears its
 * update history. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/players_globals.h"
#include "headers/simple_circular_queue.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/player_update_history.h"

#include "headers/network_game_client.h"
extern void hcex_fire_plr_event(const char *event_name, int local_player); /* DEVIATION: slot 1 is int, not short -- def src/hcex/hcex_fire_plr_event.cpp + prologue stw r4 @0x823E4730 */
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern void player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(int player_index);
extern int16_t game_connection(void);
extern void update_server_player_died(int player_index);
extern void simple_circular_queue_empty(simple_circular_queue *queue);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_clear(struct player_update_history *history);

void players_handle_deleted_object(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    if ( ((1 << object->object.type) & object_mask_unit) == 0 )   /* only bipeds/vehicles carry a player */
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player; player = data_iterator_next(&iterator) )
    {
        if ( player->unit_index != object_index )
            continue;

        int player_index_low = (uint16_t)iterator.index;
        player_datum *record = DATA_ARRAY_ELEMENT(player_data, player_datum, iterator.index);
        int player_index = iterator.index;
        int old_unit_index = record->unit_index;
        int16_t local_player = record->local_player_index;
        record->unit_index = -1;
        record->dead_unit_index = old_unit_index;
        if ( local_player != -1 )
        {
            hcex_fire_plr_event("player_died", local_player);
            player_control_new_unit(local_player, -1);
        }

        players_globals->all_dead = 1;
        data_iterator other;
        data_iterator_new(&other, player_data);
        for ( player_datum *p = data_iterator_next(&other); p; p = data_iterator_next(&other) )
        {
            if ( p->unit_index != -1 )
                players_globals->all_dead = 0;
        }

        player_datum *fixed_record = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index_low);
        if ( fixed_record->quit_out_of_game == 1 )
        {
            player_cleanup_after_player_has_quit_game_and_no_longer_attached_to_unit(player_index);
        }
        else
        {
            if ( game_connection() == _game_connection_network_server )
                update_server_player_died(player_index);
            if ( (uint16_t)fixed_record->local_player_index == 0xFFFF )   /* sentinel 0xFFFF on signed __int16 */
            {
                if ( game_connection() == _game_connection_network_client )
                {
                    simple_circular_queue_empty(&fixed_record->___u26.client_update_data.___u0.remote_player.action_queue.queue);
                    simple_circular_queue_empty(&fixed_record->___u26.client_update_data.___u0.remote_player.position_queue.queue);
                    simple_circular_queue_empty(&fixed_record->___u26.client_update_data.___u0.remote_player.vehicle_update_queue.queue);
                    fixed_record->___u26.client_update_data.___u0.remote_player.action_queue.last_valid_action.throttle.n[0] = 0.0f;
                    fixed_record->___u26.client_update_data.___u0.remote_player.action_queue.last_valid_action.throttle.n[1] = 0.0f;
                    fixed_record->___u26.client_update_data.___u0.remote_player.action_queue.last_valid_action.control_flags = 0;
                }
            }
            else if ( global_network_game_client_get() )
            {
                player_update_history_clear(nullptr);
            }
        }
    }
}
