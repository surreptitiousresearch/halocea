/* styled_font_get @0x83769F88 — resolves a font tag index plus a style variant (bold/italic/etc, or -1
 * for "no style override") to the concrete font_header to render with. If the requested style's font
 * reference is unset (index -1), or no style was requested, falls back to the base font. */

#include <stdint.h>
#include "headers/font_header.h"
#include "headers/global_tag_instances.h"

font_header * styled_font_get(int base_font_index, int16_t style)
{
    int font_index = base_font_index;

    if (style != -1)
    {
        font_header *base_font = TAG_GET(font_header, base_font_index);
        font_index = base_font->style_fonts[style].index;
    }

    if (font_index == -1)
        font_index = base_font_index;

    return TAG_GET(font_header, font_index);
}
