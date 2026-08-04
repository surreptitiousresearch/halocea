#include <stdint.h>
#include "headers/widget_globals.h"

extern unsigned char we_are_at_the_main_menu;

uint8_t transition_to_game_in_progress(void)
{
    if (!we_are_at_the_main_menu)
        return 0;
    if (widget_globals.fade_to_black > 1.0f)
        return 0;
    if (widget_globals.fade_to_black < 0.0f)
        return 0;
    return 1;
}
