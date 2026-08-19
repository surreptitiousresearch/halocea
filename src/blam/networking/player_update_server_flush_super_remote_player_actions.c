/* player_update_server_flush_super_remote_player_actions @0x83757898 — server-side flush of any
 * pending super-remote player action updates (both the stateless and incremental queues) to the
 * network at the start of a tick. */

#include "headers/super_remote_players_action_update.h"
#include "headers/blam_data_globals.h"


extern void send_super_remote_players_action_update(super_remote_players_action_update *update);

void player_update_server_flush_super_remote_player_actions(void)
{
    if ( g_stateless_super_remote_players_action_udpate.number_of_updates > 0 )
        send_super_remote_players_action_update(&g_stateless_super_remote_players_action_udpate);
    if ( g_incremental_super_remote_players_action_upate.number_of_updates > 0 )
        send_super_remote_players_action_update(&g_incremental_super_remote_players_action_upate);
}
