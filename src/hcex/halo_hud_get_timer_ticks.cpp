#include "../headers/hud_messaging_globals.h"

int halo_hud_get_timer_ticks(void)
{
    return (unsigned short)hud_messaging_globals->timer.ticks;
}
