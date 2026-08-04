#pragma once
/* weapon_interface_magazine_state — per-magazine ammo state shown on the weapon HUD. Layout from the
 * database (10 bytes). */

typedef struct weapon_interface_magazine_state
{
    unsigned __int8 reloading;                /* 0x0 */
    unsigned __int8 can_fire;                 /* 0x1 */
    __int16         rounds_loaded;            /* 0x2 */
    __int16         rounds_loaded_maximum;    /* 0x4 */
    __int16         rounds_remaining;         /* 0x6 */
    __int16         rounds_remaining_maximum; /* 0x8 */
} weapon_interface_magazine_state;            /* 10 bytes */
