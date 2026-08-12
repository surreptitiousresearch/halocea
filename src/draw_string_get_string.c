#include <stdint.h>
#include "headers/font_drawing_globals.h"
#include "headers/string_index.h"

extern char *string_list_get_string(int tag_index, int16_t string_index);

char * draw_string_get_string(int16_t index)
{
    return string_list_get_string(font_drawing_globals.string_list_index, index + _string_index_first_bitmap_text_string);
}
