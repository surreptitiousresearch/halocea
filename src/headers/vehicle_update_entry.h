#pragma once
/* vehicle_update_entry — one buffered remote-player vehicle update: the client update id it applies to,
 * a "useful in N updates" countdown, and the vehicle state payload (72 bytes, DB layout). */

#include "remote_player_vehicle_update_network_data.h"

typedef struct vehicle_update_entry
{
    int client_update_id;                              /* 0x00 */
    int updates_until_useful;                          /* 0x04 */
    remote_player_vehicle_update_network_data data;    /* 0x08 */
} vehicle_update_entry;                                /* 72 bytes */
