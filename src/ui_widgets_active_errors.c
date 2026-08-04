/* ui_widgets_active_errors @0x83730910 — true if any active widget's name contains "error_modal". */

#include <string.h>
#include "headers/widget_globals.h"

int ui_widgets_active_errors(void)
{
    int found = 0;

    if ( widget_globals.initialized )
    {
        int local_player_index;

        /* recovered: cursor pointer-walk -> active_widgets[2] index loop */
        for ( local_player_index = 0; local_player_index < 2; ++local_player_index )
        {
            widget_instance *widget = widget_globals.active_widgets[local_player_index];
            if ( widget && strstr(widget->name, "error_modal") )
                found = 1;
        }
    }
    return found;
}
