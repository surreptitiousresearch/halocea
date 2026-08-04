#include <stdint.h>
#include "headers/blam_data_globals.h"
/* main_menu_is_active @ 0x83730EA8 */
uint8_t main_menu_is_active(void)
{
    return we_are_at_the_main_menu;
}
