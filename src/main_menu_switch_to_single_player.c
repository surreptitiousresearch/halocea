/* main_menu_switch_to_single_player @0x836894F8 — request a level change out of the menu into single player. */

#include "headers/main_globals.h"

void main_menu_switch_to_single_player(void)
{
    main_globals.rename_map = 1;
}
