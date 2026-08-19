/* draw_string_set_font @0x83769BC0 */
#include "headers/font_drawing_globals.h"

void draw_string_set_font(int font_index)
{
    font_drawing_globals.current_font_index = font_index;
}
