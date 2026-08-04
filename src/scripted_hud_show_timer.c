/* scripted_hud_show_timer @0x836A4268 — enable or disable the HUD countdown timer display. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"


void scripted_hud_show_timer(uint8_t enabled)
{
    hud_messaging_globals->timer.enabled = enabled;
}
