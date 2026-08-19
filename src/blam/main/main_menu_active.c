#include <stdint.h>
#include "headers/blam_data_globals.h"
/* main_menu_active @0x83730E98 — set the "we are at the main menu" global flag. */


void main_menu_active(uint8_t active)
{
    we_are_at_the_main_menu = active;
}
