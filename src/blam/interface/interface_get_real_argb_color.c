/* interface_get_real_argb_color @0x836A64E8 — look up an ARGB color from one of the interface color tables.
 * Defaults to opaque white, then (if the referenced color-table tag is loaded and non-empty) overrides with the
 * color at color_index, wrapped modulo the table size. Returns the supplied color buffer. */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"
#include "headers/real_argb_color.h"
#include "headers/color_table_color.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


real_argb_color *interface_get_real_argb_color(int16_t interface_color_table_index, int16_t color_index,
                                               real_argb_color *color)
{
    tag_reference *interface_references = nullptr;
    if (global_game_globals->interface_tag_references.count)
        interface_references = (tag_reference *)global_game_globals->interface_tag_references.address;
    int color_table_tag_index = interface_references[interface_color_table_index].index;

    color->n[0] = 1.0f;
    color->n[1] = 1.0f;
    color->n[2] = 1.0f;
    color->n[3] = 1.0f;

    if (color_table_tag_index != -1)
    {
        int *color_table = TAG_GET(int, color_table_tag_index);
        int color_count = *color_table;
        if (color_count)
            /* color_table[1] is the color-block address; 48-byte color_table_color records (name[32] + argb@32) */
            *color = ((color_table_color *)color_table[1])[color_index % color_count].real_color;
    }
    return color;
}
