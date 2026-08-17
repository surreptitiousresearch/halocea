/* draw_string_get_color @0x83769B90 */
#include "headers/font_drawing_globals.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


void draw_string_get_color(real_argb_color *color)
{
    *color = font_drawing_globals.current_color;
}
