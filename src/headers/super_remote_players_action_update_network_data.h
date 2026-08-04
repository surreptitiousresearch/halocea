#pragma once
/* super_remote_players_action_update_network_data — the batched wire payload for up to 32 remote players'
 * action updates: a parallel array of routing headers and action bodies (1792 bytes, DB layout). */

#include "remote_player_action_update_header.h"
#include "remote_player_action_update_network_data.h"

typedef struct super_remote_players_action_update_network_data
{
    remote_player_action_update_header       headers[32]; /* 0x000 */
    remote_player_action_update_network_data actions[32]; /* 0x100 */
} super_remote_players_action_update_network_data;         /* 1792 bytes */
