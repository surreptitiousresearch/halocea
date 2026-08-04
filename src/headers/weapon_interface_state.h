#pragma once
/* weapon_interface_state — the live weapon-HUD readout for one weapon: heat/age meters, overheat state,
 * and up to two magazines of ammo. Layout from the database (32 bytes). */

#include "weapon_interface_magazine_state.h"

typedef struct weapon_interface_state
{
    float           heat;            /* 0x00 */
    float           age;             /* 0x04 */
    unsigned __int8 overheated;      /* 0x08 */
    unsigned __int8 _pad09;          /* 0x09 */
    __int16         magazine_count;  /* 0x0A */
    weapon_interface_magazine_state magazines[2]; /* 0x0C */
} weapon_interface_state;            /* 32 bytes */
