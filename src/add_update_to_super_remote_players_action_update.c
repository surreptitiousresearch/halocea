/* add_update_to_super_remote_players_action_update @0x837578F8 — appends one remote player's action update
 * (header + network data) to a batched super-update; flushes the batch to the network once it fills to 32
 * entries. */

#include "headers/super_remote_players_action_update.h"

extern void send_super_remote_players_action_update(super_remote_players_action_update *update);

void add_update_to_super_remote_players_action_update(super_remote_players_action_update *update,
    remote_player_action_update_header *header, remote_player_action_update_network_data *update_data)
{
    int number_of_updates = update->number_of_updates;
    update->network_data.headers[number_of_updates] = *header;
    update->network_data.actions[number_of_updates] = *update_data;

    if ( ++update->number_of_updates == 32 )
        send_super_remote_players_action_update(update);
}
