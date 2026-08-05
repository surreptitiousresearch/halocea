#pragma once
#include <stdint.h>
/* weapon_start_reload_network_data — wire body for replicating a weapon reload start to clients.
 * DB-verified via types_members weapon_start_reload_network_data (12 bytes). */

typedef struct weapon_start_reload_network_data
{
    int     weapon_index;           /* 0x00 — network-translated weapon object index */
    int16_t magazine_index;         /* 0x04 */
    int16_t starting_total_rounds;  /* 0x06 — weapon object data +694 */
    int16_t starting_loaded_rounds; /* 0x08 — weapon object data +696 */
} weapon_start_reload_message; /* legacy local name for weapon_start_reload_network_data */
