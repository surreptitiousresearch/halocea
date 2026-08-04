/* ui_widgets_set_fade_value @0x837307C0 — set the global UI fade-to-black amount (0 = clear, 1 = black). */

#include "headers/widget_globals.h"


void ui_widgets_set_fade_value(float value)
{
    widget_globals.fade_to_black = value;
}
