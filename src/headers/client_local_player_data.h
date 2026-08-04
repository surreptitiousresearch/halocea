#pragma once
/* client_local_player_data — DB type, 20 bytes (types_members). Client-side prediction state for
 * the local player: last update sequence/ack plus the server's position at the last acked update. */

#include "real_point3d.h"

typedef struct client_local_player_data
{
    int          last_update_sequence_number;          /* 0x00 */
    int          last_acked_update_id;                 /* 0x04 */
    real_point3d server_position_as_of_last_acked_update; /* 0x08 */
} client_local_player_data; /* 20 bytes */
