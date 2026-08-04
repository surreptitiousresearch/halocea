#pragma once
/* server_update_packet — decoded film body packet (a server game update). Rebound to the DB type
 * message_server_game_update: the former ad-hoc fields (update_id/_f1/_f2/crc/_f10/number_of_actions/
 * actions) are its update_number/debug_random_seed/debug_game_time/game_state_crc/pad/player_count/
 * action_update members. */

#include "player_action.h"

typedef struct message_server_game_update
{
    unsigned int     update_number;     /* 0x00 */
    unsigned int     debug_random_seed; /* 0x04 */
    unsigned int     debug_game_time;   /* 0x08 */
    unsigned int     game_state_crc;    /* 0x0C */
    unsigned __int16 pad;               /* 0x10 */
    __int16          player_count;      /* 0x12 */
    player_action    action_update[32]; /* 0x14 */
} message_server_game_update;           /* 1044 bytes */

typedef message_server_game_update server_update_packet;
