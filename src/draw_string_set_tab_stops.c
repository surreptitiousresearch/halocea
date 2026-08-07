/* draw_string_set_tab_stops @0x83769B08 — install up to 16 tab-stop positions for subsequent string draws.
 * The count is clamped to 16; a count <= 0 leaves the stops untouched (only the count is updated). */

#include <stdint.h>
#include <string.h>
#include "headers/font_drawing_globals.h"
#include "headers/blam_data_globals.h"


void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count)
{
    int16_t clamped = count;
    if (count > 16)
        clamped = 16;

    font_drawing_globals.tab_stop_count = clamped;
    if (clamped > 0)
        memcpy(font_drawing_globals.tab_stops, tab_stops, 2 * clamped);
}
