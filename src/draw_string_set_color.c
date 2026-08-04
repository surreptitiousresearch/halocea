/* draw_string_set_color @0x83769B60 — set the current text color used by subsequent string draws. */

#include "headers/font_drawing_globals.h"
#include "headers/blam_data_globals.h"


void draw_string_set_color(const real_argb_color *color)
{
    font_drawing_globals.current_color = *color;
}
