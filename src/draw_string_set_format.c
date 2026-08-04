#include <stdint.h>
#include "headers/font_drawing_globals.h"

void draw_string_set_format(int16_t style, int16_t justification, unsigned int flags)
{
    font_drawing_globals.current_style         = style;
    font_drawing_globals.current_justification = justification;
    font_drawing_globals.current_flags         = flags;
}
