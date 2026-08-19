/* interface_set_bitmap_text_draw_mode @0x836A65B0 — configure the string drawer for a bitmap-font text pass:
 * resolve the interface font's tag index from the game globals' interface reference table, look up the requested
 * color, and push the draw mode (font, style, justification, flags, color) into the string drawer. */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"


extern real_argb_color *interface_get_real_argb_color(int16_t interface_color_table_index, int16_t color_index, real_argb_color *color);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);

void interface_set_bitmap_text_draw_mode(int16_t interface_font_index, int16_t style, int16_t justification,
                                         unsigned int flags, int16_t color_table_index, int16_t color_index)
{
    tag_reference *interface_references = nullptr;
    if (global_game_globals->interface_tag_references.count)
        interface_references = (tag_reference *)global_game_globals->interface_tag_references.address;

    int font_tag_index = interface_references[interface_font_index].index;

    real_argb_color color;
    interface_get_real_argb_color(color_table_index, color_index, &color);
    draw_string_set_draw_mode(font_tag_index, style, justification, flags, &color);
}
