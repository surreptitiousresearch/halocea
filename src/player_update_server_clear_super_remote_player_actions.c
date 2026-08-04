#include "headers/super_remote_players_action_update.h"
#include "headers/blam_data_globals.h"


void player_update_server_clear_super_remote_player_actions(void)
{
    g_stateless_super_remote_players_action_udpate.number_of_updates = 0;
    g_incremental_super_remote_players_action_upate.number_of_updates = 0;
}
