/* scripted_hud_blink_motion_sensor @0x836D67D8 — script override to blink/steady the motion sensor on the HUD (unit_hud_globals script_flags bit
 * 0x20: set = blinking). */

#include <stdint.h>
#include "headers/unit_hud_globals.h"
#include "headers/hud_panel_flags.h"
#include "headers/blam_data_globals.h"


void scripted_hud_blink_motion_sensor(uint8_t blink)
{
    int flags = unit_hud_globals->script_flags;
    unit_hud_globals->script_flags = blink ? (flags | (1u << _hud_panel_motion_sensor_blink_bit))
                                           : (flags & ~(1u << _hud_panel_motion_sensor_blink_bit));
}
