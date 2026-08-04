/* ui_widgets_force_fullscreen @0x83730980 — when the widget system is up, scan the two active widget
 * slots for an "error_modal" widget (so an error dialog can force the UI fullscreen). The scan's strstr
 * result is discarded in this build, leaving the routine as a query that always reports "no" by clearing
 * the caller's local-player reference to -1.
 *
 * DEVIATION: the decompiler walks a raw _widget_globals* by 4-byte steps from &widget_globals up to
 * widget_globals.widget_stack; that span is exactly active_widgets[0..1], reproduced as an index loop. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"


extern char *strstr(const char *haystack, const char *needle);

uint8_t ui_widgets_force_fullscreen(int16_t *local_player_index_reference)
{
    if (widget_globals.initialized)
    {
        for (int slot = 0; slot < 2; ++slot)
        {
            if (widget_globals.active_widgets[slot])
                strstr(widget_globals.active_widgets[slot]->name, "error_modal");
        }
    }

    if (local_player_index_reference)
        *local_player_index_reference = -1;
    return 0;
}
