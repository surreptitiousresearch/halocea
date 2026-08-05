#pragma once
/* remote_player_action_update_network_data — the body of one remote player's action update: the control
 * action to apply, how many ticks to apply it over, and the player's facing vector (48 bytes, DB layout). */

#include <stdint.h>
#include "player_action.h"
#include "real_vector3d.h"

typedef struct remote_player_action_update_network_data
{
    uint8_t ticks_to_apply_update_to; /* 0x00 */
    unsigned char   _pad1[3];                 /* 0x01 */
    player_action   action;                   /* 0x04 */
    real_vector3d   facing_vector;            /* 0x24 */
} remote_player_action_update_network_data;   /* 48 bytes */
