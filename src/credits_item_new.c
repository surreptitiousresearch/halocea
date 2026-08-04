/* credits_item_new @0x83697FA8 — append one line to the credits scroll list (20-byte elements: text,
 * string index, heading flag, top y, bottom y) and return its height in pixels. Escape prefixes in the
 * text: "\sN" makes an N*8-half-line spacer (empty text, height 16*N), "\h" marks a heading (prefix
 * stripped). A normal line is 8 units tall (16 pixels returned). Empty strings add nothing. */

#include "headers/dynamic_array.h"
#include "headers/credits_line_s.h"
#include "headers/blam_data_globals.h"

extern wchar_t empty_wide_string;   /* the empty wide string used for spacer rows */

extern unsigned int ustrlen(const wchar_t *string);
extern int uatoi(const wchar_t *string);
extern int dynamic_array_add_element(dynamic_array *array);

int credits_item_new(const wchar_t *string, int string_index, int y0)
{
    if ( !ustrlen(string) )
        return 0;

    credits_line_s *item = &((credits_line_s *)credits_list.elements)[dynamic_array_add_element(&credits_list)];
    int height = 8;
    item->y0 = y0;
    item->string_index = string_index;
    item->formatting = 0;

    if ( *string == L'\\' )
    {
        wchar_t escape = *++string;
        if ( escape == L's' )
        {
            height = 8 * uatoi(string + 1);
            item->string = &empty_wide_string;
            item->y1 = y0 + 2 * height;
            return height * 2;
        }
        if ( escape == L'h' )
        {
            ++string;
            item->formatting = 1;
        }
    }

    item->string = string;
    item->y1 = y0 + 16;
    return height * 2;
}
