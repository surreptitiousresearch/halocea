/* player_delete @0x836A7D08 — remove a player: notify the game engine, and on a client (connection 1)
 * shut down the player's remote-update action/position queues when it was not a local player;
 * on a server (connection 2) notify the network server. Then clear the machine->player table slot and
 * delete the player datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/action_queue.h"
#include "headers/position_queue.h"
#include "headers/blam_data_globals.h"

extern void game_engine_player_removed(int player_index);
extern int16_t game_connection(void);
extern void *datum_try_and_get(const data_array *data, int index);
extern void datum_delete(data_array *data, int index);
extern void action_queue_shutdown(action_queue *queue);
extern void position_queue_shutdown(position_queue *queue);
extern network_game_server *global_network_game_server_get(void);

void player_delete(int machine_index, int player_index)
{
    char processed = 0;
    game_engine_player_removed(player_index);

    int16_t connection = game_connection();
    if ( connection == 1 )
    {
        player_datum *player = datum_try_and_get(player_data, player_index);
        if ( player && player->local_player_index == -1 )   /* remote player */
        {
            action_queue_shutdown(&player->___u26.client_update_data.___u0.remote_player.action_queue);
            position_queue_shutdown(&player->___u26.client_update_data.___u0.remote_player.position_queue);
        }
        processed = 1;
    }
    else if ( connection == 2 )
    {
        global_network_game_server_get();
        processed = 1;
    }

    if ( processed )
    {
        /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
        int slot = 0;
        int *entry = &machine_to_player_table[2 * (machine_index & 0xFFFF)];
        while ( *entry != player_index )
        {
            ++slot;
            ++entry;
            if ( slot >= 2 )
                goto done;
        }
        machine_to_player_table[2 * (machine_index & 0xFFFF) + slot] = -1;
    }
done:
    datum_delete(player_data, player_index);
}
