/* main_menu_screen_is_active @ 0x83730EB8 — true when the top widget is "the_main_menu" */
#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"

uint8_t main_menu_screen_is_active(void)
{
    const char *name;
    const char *target = "the_main_menu";

    if ( we_are_at_the_main_menu != 1 || !widget_globals.active_widgets[0] )
        return 0;

    name = widget_globals.active_widgets[0]->name;
    /* inline strcmp(name, "the_main_menu") == 0 */
    while ( *name && *(const unsigned char *)name == *(const unsigned char *)target )
    {
        ++name;
        ++target;
    }
    return *(const unsigned char *)name == *(const unsigned char *)target;
}
