#pragma once
/* scenario_starting_profile — one entry in the scenario's starting_profiles tag_block (104 bytes).
 * DB-verified layout (types_members scenario_starting_profile). */

#include "starting_weapon_info.h"

typedef struct scenario_starting_profile
{
    char                name[32];                     /* 0x00 */
    float               starting_health;              /* 0x20 */
    float               starting_shield;              /* 0x24 */
    starting_weapon_info starting_weapons[2];         /* 0x28 */
    unsigned __int8     starting_grenade_counts[4];   /* 0x50 */
    int                 unused[5];                    /* 0x54 */
} scenario_starting_profile;                          /* 0x68 = 104 bytes */
