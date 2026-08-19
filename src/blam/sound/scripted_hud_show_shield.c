/* scripted_hud_show_shield @0x836D6760 — script override to show/hide the shield bar on the HUD
 * (unit_hud_globals script_flags _hud_panel_shield_dont_show_bit: set = hidden). */

#include <stdint.h>
#include "headers/unit_hud_globals.h"
#include "headers/hud_panel_flags.h"
#include "headers/blam_data_globals.h"


void scripted_hud_show_shield(uint8_t show)
{
    int flags = unit_hud_globals->script_flags;
    unit_hud_globals->script_flags = show
        ? (flags & ~(1u << _hud_panel_shield_dont_show_bit))
        : (flags | (1u << _hud_panel_shield_dont_show_bit));
}
