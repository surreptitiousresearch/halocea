#pragma once
#include <stdint.h>
/* player_exit_vehicle_network_data — wire body for replicating a unit exiting a vehicle seat to
 * clients. DB-verified via types_members player_exit_vehicle_network_data (8 bytes). */

typedef struct player_exit_vehicle_network_data
{
    int             object_index; /* 0x0 — network-translated unit object index */
    uint8_t forced_exit;  /* 0x4 */
} player_exit_vehicle_message; /* legacy local name for player_exit_vehicle_network_data */
