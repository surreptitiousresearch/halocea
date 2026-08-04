/* parse_string_new @0x83769FD8 — initialise a parse_string_state for laying out/drawing a markup string.
 * Stores style/justification/string/base font, packs the float color into a 0xAARRGGBB integer, and
 * resolves the font_header to use: with no style override (style == -1) the base font is used; otherwise the
 * base font's style table is indexed to find the styled font tag, falling back to the base font if the slot
 * is empty. */

#include <stdint.h>
#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/real_argb_color.h"
#include "headers/global_tag_instances.h"

void parse_string_new(parse_string_state *state, const char *string, int base_font_index, int16_t style,
                      int16_t justification, const real_argb_color *color)
{
    state->style = style;
    state->justification = justification;
    state->string = (unsigned __int8 *)string;
    state->string_index = 0;
    state->base_font_index = base_font_index;

    /* Decompiler used __int64 intermediates for the float->int conversions; each channel is 0..255 so plain
     * int truncation is equivalent. Packs as (alpha<<24)|(red<<16)|(green<<8)|blue. */
    unsigned int alpha = (int)(color->n[0] * 255.0f);
    unsigned int red   = (int)(color->n[1] * 255.0f);
    unsigned int green = (int)(color->n[2] * 255.0f);
    unsigned int blue  = (int)(color->n[3] * 255.0f);
    state->color = (((((alpha << 8) | red) << 8) | green) << 8) | blue;

    int font_index;
    if (style == -1)
        font_index = base_font_index;
    else
        font_index = TAG_GET(int, base_font_index)[4 * style + 18];
    if (font_index == -1)
        font_index = base_font_index;

    state->font_header = TAG_GET(font_header, font_index);
}
