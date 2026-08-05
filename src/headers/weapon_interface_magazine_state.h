#pragma once
#include <stdint.h>
/* weapon_interface_magazine_state — per-magazine ammo state shown on the weapon HUD. Layout from the
 * database (10 bytes). */

typedef struct weapon_interface_magazine_state
{
    uint8_t reloading;                /* 0x0 */
    uint8_t can_fire;                 /* 0x1 */
    int16_t         rounds_loaded;            /* 0x2 */
    int16_t         rounds_loaded_maximum;    /* 0x4 */
    int16_t         rounds_remaining;         /* 0x6 */
    int16_t         rounds_remaining_maximum; /* 0x8 */
} weapon_interface_magazine_state;            /* 10 bytes */
