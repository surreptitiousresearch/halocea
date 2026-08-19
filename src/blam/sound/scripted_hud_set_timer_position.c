/* scripted_hud_set_timer_position @0x836A4228 — set the HUD timer's screen position (x,y) and anchor corner
 * (clamped to 0..4; negative corner resets to 0). */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_corner.h"
#include "headers/blam_data_globals.h"


void scripted_hud_set_timer_position(int16_t x, int16_t y, int16_t corner)
{
    hud_messaging_globals->timer.position.n[0] = x;
    hud_messaging_globals->timer.position.n[1] = y;
    if ( corner >= 0 )
        hud_messaging_globals->timer.corner = corner > 4 ? 4 : corner;
    else
        hud_messaging_globals->timer.corner = _hud_corner_top_left;
}
