#pragma once
/* game_globals_grenade — one entry of game_globals.grenades (the 'matg' per-grenade-type table):
 * max carried count, default multiplayer spawn count, and tag references for the throwing effect,
 * HUD interface, item, and thrown projectile. Layout from the database (68 bytes). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct game_globals_grenade
{
    int16_t       maximum_count;      /* 0x00 */
    int16_t       mp_spawn_default;   /* 0x02 */
    tag_reference throwing_effect;    /* 0x04 */
    tag_reference hud_interface;      /* 0x14 */
    tag_reference item;               /* 0x24 */
    tag_reference projectile;         /* 0x34 */
} game_globals_grenade; /* 0x44 (68 bytes) */
