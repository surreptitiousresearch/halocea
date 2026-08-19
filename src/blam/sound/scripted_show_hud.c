/* scripted_show_hud @0x83707E90 — script override for overall HUD visibility; returns the new state. */

#include <stdint.h>
#include "headers/hud_scripted_globals.h"
#include "headers/blam_data_globals.h"


uint8_t scripted_show_hud(uint8_t flag)
{
    hud_scripted_globals->show_hud = flag;
    return hud_scripted_globals->show_hud;
}
