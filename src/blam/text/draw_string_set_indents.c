/* draw_string_set_indents @0x83769B48 */
#include <stdint.h>
#include "headers/font_drawing_globals.h"
#include "headers/blam_data_globals.h"


void draw_string_set_indents(int16_t initial_indent, int16_t paragraph_indent)
{
    font_drawing_globals.initial_indent = initial_indent;
    font_drawing_globals.paragraph_indent = paragraph_indent;
}
