#pragma once
/* unit_drop_current_weapon_network_data — wire body for replicating a unit dropping its current weapon
 * to client machines. Unit/weapon indices are wire-translated before packing.
 * DB-verified via types_members unit_drop_current_weapon_network_data (12 bytes). */

typedef struct unit_drop_current_weapon_network_data
{
    int             unit_index;      /* 0x00 — network-translated */
    int             weapon_to_drop;  /* 0x04 — network-translated */
    unsigned __int8 immediate;       /* 0x08 */
} unit_drop_weapon_message; /* legacy local name for unit_drop_current_weapon_network_data */
