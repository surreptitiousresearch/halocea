/* scripted_hud_show_crosshair @0x836AE990 — script override to show/hide the weapon crosshair
 * (weapon_hud_globals script_flags bit 0x1: set = shown). */

#include <stdint.h>
#include "headers/weapon_hud_globals.h"
#include "headers/hud_crosshair_show_flags.h"
#include "headers/blam_data_globals.h"


void scripted_hud_show_crosshair(uint8_t show)
{
    /* recovered: *(int *)((char *)weapon_hud_globals + 0xF0) -> weapon_hud_globals->script_flags */
    weapon_hud_globals->script_flags = show
        ? (weapon_hud_globals->script_flags | (1u << _hud_crosshair_show_bit))
        : (weapon_hud_globals->script_flags & ~(1u << _hud_crosshair_show_bit));
}
