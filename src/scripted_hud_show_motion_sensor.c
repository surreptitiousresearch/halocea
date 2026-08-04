/* scripted_hud_show_motion_sensor @0x836D67B0 — script override to show/hide the motion sensor on the HUD
 * (unit_hud_globals script_flags _hud_panel_motion_sensor_dont_show_bit: set = hidden). */

#include <stdint.h>
#include "headers/unit_hud_globals.h"
#include "headers/hud_panel_flags.h"
#include "headers/blam_data_globals.h"


void scripted_hud_show_motion_sensor(uint8_t show)
{
    int flags = unit_hud_globals->script_flags;
    unit_hud_globals->script_flags = show
        ? (flags & ~(1u << _hud_panel_motion_sensor_dont_show_bit))
        : (flags | (1u << _hud_panel_motion_sensor_dont_show_bit));
}
