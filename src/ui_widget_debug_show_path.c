/* ui_widget_debug_show_path @0x837307D0 — set the debug flag that draws widget navigation paths. */

#include <stdint.h>
#include "headers/widget_globals.h"

void ui_widget_debug_show_path(uint8_t show)
{
    widget_globals.debug_show_path = show;
}
