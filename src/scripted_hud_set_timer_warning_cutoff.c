/* scripted_hud_set_timer_warning_cutoff @0x836A4200 — set the remaining-time threshold (in ticks) below which
 * the HUD timer flashes, from a minutes/seconds value (30 ticks per second). */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"


void scripted_hud_set_timer_warning_cutoff(int16_t minutes, int16_t seconds)
{
    hud_messaging_globals->timer.flash_cutoff = 30 * (60 * minutes + seconds);
}
