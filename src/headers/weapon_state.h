#pragma once
/* weapon_state — networked snapshot of one seat/weapon slot; only a validity flag is modelled here (all this
 * subsystem records). Layout from the database (1 byte). */

typedef struct weapon_state
{
    unsigned __int8 is_valid_weapon;  /* 0x00 */
} weapon_state;                        /* 1 byte */
