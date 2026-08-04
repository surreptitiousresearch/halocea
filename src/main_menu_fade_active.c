/* main_menu_fade_active @0x836894E0 — true while a main-menu map-change fade is in progress. */

#include <stdint.h>
#include "headers/main_globals.h"

/* lwz map_change_load_timer, boolean via addic/subfe; caller clrlwi r3,24 -> uint8_t */
uint8_t main_menu_fade_active(void)
{
    return main_globals.map_change_load_timer != 0;
}
