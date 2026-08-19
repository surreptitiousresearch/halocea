/* ui_widgets_active @ 0x83730868 — true if any controller currently has a root widget up. */

#include <stdint.h>
#include "headers/widget_globals.h"

uint8_t ui_widgets_active(void)
{
    int local_player_index;

    if ( !widget_globals.initialized )
        return 0;

    /* recovered: pointer-walk over &widget_globals..widget_stack -> active_widgets[2] index loop */
    for ( local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        if ( widget_globals.active_widgets[local_player_index] )
            return 1;
    }
    return 0;
}
