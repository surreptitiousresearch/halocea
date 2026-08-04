/* main_fade_to_dashboard @0x83688ED0 — request that the game fade out and return to the Xbox dashboard. */

#include "headers/main_globals.h"

void main_fade_to_dashboard(void)
{
    main_globals.want_to_be_at_main_menu = 0;
    main_globals.fade_to_dashboard = 1;
    main_globals.rename_map = 1;
}
