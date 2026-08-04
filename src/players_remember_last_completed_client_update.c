/* players_remember_last_completed_client_update @0x836A89A8 — server-side: after simulating, record
 * for each player whose unit just completed a client update the update sequence number and the unit's
 * (or its vehicle parent's) world position, then queue that remote-player position update for network
 * send. Non-local players additionally log the completion.
 *
 * Deviation: the decompiler's void* return (the last iterator result) is discarded — void. */

#include <stdint.h>
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern int game_time_get(void);
extern unsigned int GetTickCount(void);
extern void player_update_server_log(const char *format, ...);
extern void player_update_server_remote_player_position_update_to_network(int player_index);

void players_remember_last_completed_client_update(void)
{
    data_iterator iter;
    player_datum *player;

    if ( game_connection() != _game_connection_network_server )
        return;

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player;
          player = data_iterator_next(&iter) )
    {
        int unit_index = player->unit_index;
        unit_datum *unit;
        if ( unit_index == -1 )
            continue;

        unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

        if ( unit->unit.did_just_complete_client_update != 1 )
            continue;

        player->___u26.server_update_data.last_completed_update_id = unit->unit.last_completed_client_update_id;
        unit->unit.did_just_complete_client_update = 0;

        if ( unit->object.parent_object_index == -1 )
        {
            player->___u26.server_update_data.position_as_of_last_completed_update = unit->object.position;
        }
        else
        {
            int parent_index = unit->object.parent_object_index;
            unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum);
            player->___u26.server_update_data.position_as_of_last_completed_update = parent->object.position;
        }

        if ( (unsigned __int16)player->local_player_index == 0xFFFF )
        {
            unsigned int tick_count = GetTickCount();
            int sim_time = game_time_get();
            player_update_server_log(
                "[%d]: [%d]:\t Completed [%d] ([%f] [%f] [%f]), ([%f] [%f]), ([%f] [%f])\n",
                tick_count,
                sim_time,
                player->___u26.server_update_data.last_completed_update_id,
                player->___u26.server_update_data.position_as_of_last_completed_update.n[0],
                player->___u26.server_update_data.position_as_of_last_completed_update.n[1],
                player->___u26.server_update_data.position_as_of_last_completed_update.n[2],
                unit->unit.throttle.n[0],
                unit->unit.throttle.n[1],
                unit->object.translational_velocity.n[0],
                unit->object.translational_velocity.n[1]);
        }

        player_update_server_remote_player_position_update_to_network(iter.index);
    }
}
