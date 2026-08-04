#pragma once
/* starting_weapon_info — one weapon entry in a scenario player-starting-profile: the weapon tag and its
 * loaded/total ammo (20 bytes). Layout from the database. */

#include "tag_reference.h"

typedef struct starting_weapon_info
{
    tag_reference weapon;        /* 0x00 */
    __int16       rounds_loaded; /* 0x10 */
    __int16       rounds_total;  /* 0x12 */
} starting_weapon_info;          /* 20 bytes */
