/* display_error_when_main_menu_loaded @ 0x83731000 — latch a deferred error code to be
 * shown once the main menu finishes loading (first error wins). */
#include <stdint.h>
#include "headers/widget_globals.h"
void display_error_when_main_menu_loaded(int16_t error_code)
{
    if ( (unsigned short)widget_globals.main_menu_deferred_error_code == 0xFFFF )
        widget_globals.main_menu_deferred_error_code = error_code;
}
