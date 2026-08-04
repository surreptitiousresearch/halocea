#pragma once
/* remote_player_position_update_network_data — the body of a remote-player position update message: just
 * the player's position (12 bytes, DB layout). */

#include "real_point3d.h"

typedef struct remote_player_position_update_network_data
{
    real_point3d position;   /* 0x00 */
} remote_player_position_update_network_data;  /* 12 bytes */
