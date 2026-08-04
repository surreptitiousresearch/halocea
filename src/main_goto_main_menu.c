/* main_goto_main_menu @ 0x83689430 — request a return to the main menu next frame. */

#include "headers/main_globals.h"

void main_goto_main_menu(void)
{
    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.save_map = 0;
    main_globals.want_to_be_at_main_menu = 1;
}
