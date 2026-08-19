/* draw_string_dispose_from_old_map @0x837698F8 — per-map teardown: clear the bound
 * string-list reference used by text drawing. */

#include "headers/font_drawing_globals.h"

void draw_string_dispose_from_old_map(void)
{
    font_drawing_globals.string_list_index = -1;
}
