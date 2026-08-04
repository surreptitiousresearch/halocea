/* draw_string_set_draw_mode @0x83769BE8 — set the current font, color, style, justification and flags used by
 * subsequent text drawing. */

#include <stdint.h>
#include "headers/font_drawing_globals.h"
#include "headers/real_argb_color.h"

void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags,
                               const real_argb_color *color)
{
    font_drawing_globals.current_font_index = font_index;
    font_drawing_globals.current_color = *color;
    font_drawing_globals.current_style = style;
    font_drawing_globals.current_justification = justification;
    font_drawing_globals.current_flags = flags;
}
