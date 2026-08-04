#pragma once
/* weapon_hud_state — per-local-player runtime flash timing for the weapon HUD. One flash-reference timestamp
 * per weapon HUD state (game_time_get() when the state first started flashing, or -1 when not flashing).
 * Layout from the database (40 bytes). */

typedef struct weapon_hud_state
{
    int last_weapon_flash_time[8];   /* 0x00 */
    int last_weapon_index;           /* 0x20 */
    int last_grenade_flash_time;     /* 0x24 */
} weapon_hud_state;                  /* 40 bytes */
