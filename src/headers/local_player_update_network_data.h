#pragma once
/* local_player_update_network_data — wire body of a "local player update" message-delta (16 bytes, DB
 * layout): the update sequence number, the id of the last completed update, and the player's position. */

#include "real_point3d.h"

typedef struct local_player_update_network_data
{
    unsigned __int8 sequence_number;          /* 0x0 */
    unsigned __int8 last_completed_update_id; /* 0x1 */
    unsigned char   _pad2[2];                 /* 0x2 */
    real_point3d    position;                 /* 0x4 */
} local_player_update_network_data;           /* 16 bytes */
