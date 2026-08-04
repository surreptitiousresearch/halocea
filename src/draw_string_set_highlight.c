/* draw_string_set_highlight @0x83769C28 — sets the current text-highlight range. */

#include <stdint.h>
#include "headers/font_drawing_globals.h"

void draw_string_set_highlight(int16_t start_index, int16_t stop_index)
{
    font_drawing_globals.highlight_start_index = start_index;
    font_drawing_globals.highlight_stop_index = stop_index;
}
