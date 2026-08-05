/* draw_string_initialize_for_new_map @0x83769878 — load the string-list tag used for text drawing, derive the
 * active language code from its first string, and reset the default text-drawing state. */

#include <stdint.h>
#include <stdlib.h>
#include "headers/font_drawing_globals.h"
#include "headers/interface_tag_index.h"

extern int interface_get_tag_index(int16_t interface_tag_index);
extern char *string_list_get_string(int tag_index, int16_t string_index);
extern void set_language_code(int16_t language_code);

void draw_string_initialize_for_new_map(void)
{
    int tag_index = interface_get_tag_index(_interface_string_list_localization);
    font_drawing_globals.string_list_index = tag_index;
    if ( tag_index != -1 )
    {
        const char *language_string = string_list_get_string(tag_index, 0);
        int16_t language_code = atoi(language_string);
        set_language_code(language_code);
        font_drawing_globals.current_font_index = -1;
        font_drawing_globals.tab_stop_count = 0;
        font_drawing_globals.current_flags = 0;
        font_drawing_globals.current_justification = 0;
        font_drawing_globals.initial_indent = 0;
        font_drawing_globals.paragraph_indent = 0;
    }
}
