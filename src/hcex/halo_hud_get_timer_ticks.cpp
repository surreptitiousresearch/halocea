#include "../headers/hud_messaging_globals.h"

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `halo_hud_get_timer_ticks` (@0x836A4320);
// compiled as C++ without it the definition mangles to `?halo_hud_get_timer_ticks@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" int halo_hud_get_timer_ticks(void)
{
    return (unsigned short)hud_messaging_globals->timer.ticks;
}
