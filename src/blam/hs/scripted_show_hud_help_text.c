/* scripted_show_hud_help_text @0x83707EA8 — script override for HUD help-text visibility; returns the new
 * state. */

#include <stdint.h>
#include "headers/hud_scripted_globals.h"
#include "headers/blam_data_globals.h"


uint8_t scripted_show_hud_help_text(uint8_t flag)
{
    hud_scripted_globals->show_hud_help_text = flag;
    return hud_scripted_globals->show_hud_help_text;
}
