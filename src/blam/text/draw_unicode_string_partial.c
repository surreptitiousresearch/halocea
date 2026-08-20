/* draw_unicode_string_partial @0x8376A6F8 — wide-character counterpart of draw_string_partial. Identical
 * layout/clip/blit logic, differing only in the string element type and that it tokenizes through
 * parse_unicode_string. See draw_string_partial for the algorithm. rectangle2d layout is [y0, x0, y1, x1].
 *
 * DEVIATION: the call site passes ten arguments, not eight - the clipped glyph extents occupy the ninth and
 * tenth parameter slots: sth r6, r1+0x56 @0x8376A960 (clipped bitmap_width) and sth r3, r1+0x5E @0x8376A958
 * (clipped bitmap_height). */

#include <stdint.h>
#include <stddef.h>
#include "headers/parse_string_state.h"
#include "headers/font_drawing_globals.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/point2d.h"
#include "headers/rectangle2d.h"
#include "headers/real_argb_color.h"

#include "headers/real_argb_color.h"
extern void parse_string_new(parse_string_state *state, const char *string, int base_font_index, int16_t style, int16_t justification, const real_argb_color *color);
extern uint16_t parse_unicode_string(parse_string_state *state);
extern font_character *font_get_character_by_ascii_code(font_header *header, uint16_t character);

void draw_unicode_string_partial(
    void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int color,
                           int16_t dest_x, int16_t dest_y, int16_t source_x, int16_t source_y,
                           int16_t width, int16_t height),
    point2d *cursor, const rectangle2d *bounds, const rectangle2d *clip, unsigned int color,
    const wchar_t *string, int16_t string_index, int16_t string_length)
{
    int clip_left = -32768;
    int clip_top = -32768;
    int16_t clip_right = 0x7FFF;
    int16_t clip_bottom = 0x7FFF;

    if (bounds)
    {
        if (bounds->n[1] != -32768) clip_left = (uint16_t)bounds->n[1];
        if (bounds->n[3] != 0x7FFF)  clip_right = bounds->n[3];
        if (bounds->n[0] != -32768) clip_top = (uint16_t)bounds->n[0];
        if (bounds->n[2] != 0x7FFF)  clip_bottom = bounds->n[2];
    }
    if (clip)
    {
        if (clip->n[1] > (int16_t)clip_left)  clip_left = (uint16_t)clip->n[1];
        if (clip->n[3] < clip_right)          clip_right = clip->n[3];
        if (clip->n[0] > (int16_t)clip_top)   clip_top = (uint16_t)clip->n[0];
        if (clip->n[2] < clip_bottom)         clip_bottom = clip->n[2];
    }

    int region_right = clip_right;
    if ((int16_t)clip_left >= clip_right)
        return;
    int region_bottom = clip_bottom;
    if ((int16_t)clip_top >= clip_bottom)
        return;

    parse_string_state state;
    parse_string_new(&state, (const char *)string, font_drawing_globals.current_font_index,
                     font_drawing_globals.current_style, font_drawing_globals.current_justification,
                     &font_drawing_globals.current_color);

    for (state.string_index = string_index; state.string_index < string_length; )
    {
        int character_index = state.string_index;
        unsigned int character_color =
            (character_index >= font_drawing_globals.highlight_start_index
             && character_index < font_drawing_globals.highlight_stop_index)
                ? (color ^ 0xFFFFFF)
                : color;

        parse_unicode_string(&state);
        font_character *glyph = font_get_character_by_ascii_code(state.font_header, state.character);
        if (!glyph)
            continue;

        int source_x = 0;
        int source_y = 0;
        int16_t bitmap_width = glyph->bitmap_width;
        int16_t bitmap_height = glyph->bitmap_height;
        int16_t cursor_y = cursor->n[1];
        int destination_x = (int16_t)(cursor->n[0] - glyph->bitmap_origin_x);
        int destination_x_unclipped = destination_x;
        int destination_y = (int16_t)(cursor_y - glyph->bitmap_origin_y);

        cursor->n[0] = glyph->character_width + cursor->n[0];

        if (bitmap_width + destination_x > region_right)
            bitmap_width = region_right - destination_x;
        if (destination_x < (int16_t)clip_left)
        {
            destination_x = clip_left;
            source_x = (int16_t)(clip_left - destination_x_unclipped);
            bitmap_width -= source_x;
        }

        if (bitmap_height + (int16_t)destination_y > region_bottom)
            bitmap_height = region_bottom - destination_y;
        if ((int16_t)destination_y < (int16_t)clip_top)
        {
            destination_y = clip_top;
            source_y = (int16_t)(clip_top - (cursor_y - glyph->bitmap_origin_y));
            bitmap_height -= source_y;
        }

        if (bitmap_width > 0 && bitmap_height > 0)
            draw_character(&state, state.font_header, glyph, character_color,
                           destination_x, destination_y, source_x, source_y,
                           bitmap_width, bitmap_height);
    }
}
