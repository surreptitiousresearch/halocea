/* halo_hud_get_timer_enabled @0x836A4310 — return whether the scripted-HUD countdown timer is currently
 * enabled. */

#include "../headers/hud_messaging_globals.h"


extern "C" int halo_hud_get_timer_enabled(void)
{
    return hud_messaging_globals->timer.enabled;
}
