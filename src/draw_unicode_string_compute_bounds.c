/* draw_unicode_string_compute_bounds @0x8376B3A0 — measure a unicode string laid out within `bounds`,
 * returning the tight text bounding rectangle (text_bounds) and the end-of-string cursor rectangle
 * (cursor_bounds). It resolves the active font (applying the current style's font variant if any),
 * runs the normal layout pass with a measuring callback that accumulates min/max extents into
 * text_bounds_globals, then reads the accumulated extents and the final pen position back out. */

#include <wchar.h>
#include <stdint.h>
#include "headers/font_drawing_globals.h"
#include "headers/text_bounds_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/font_header.h"
#include "headers/blam_data_globals.h"


#include "headers/parse_string_state.h"
#include "headers/font_character.h"
#include "headers/font_character.h"
extern void text_bounds_draw_character(struct parse_string_state *parse_state, font_header *font_header, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy);
extern void draw_unicode_string(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust, const wchar_t *string);

void draw_unicode_string_compute_bounds(const rectangle2d *bounds, const wchar_t *string,
                                        rectangle2d *text_bounds, rectangle2d *cursor_bounds)
{
    int font_index = font_drawing_globals.current_font_index;

    /* Reset the accumulator to inverted extremes (y0/x0 = SHRT_MAX, y1/x1 = SHRT_MIN). */
    text_bounds_globals.bounds.n[0] = 32767;
    text_bounds_globals.bounds.n[1] = 32767;
    text_bounds_globals.bounds.n[2] = -32768;
    text_bounds_globals.bounds.n[3] = -32768;

    int style_font_index;
    if (font_drawing_globals.current_style == -1)
        style_font_index = font_drawing_globals.current_font_index;
    else
        style_font_index = TAG_GET(int, font_drawing_globals.current_font_index)
                               [4 * font_drawing_globals.current_style + 18];
    if (style_font_index != -1)
        font_index = style_font_index;

    text_bounds_globals.last_font_header = TAG_GET(font_header, font_index);

    point2d pen;
    draw_unicode_string(text_bounds_draw_character, bounds, &pen, nullptr, 0, string);

    font_header *last_font_header = text_bounds_globals.last_font_header;

    cursor_bounds->n[1] = pen.__s1.x;
    cursor_bounds->n[3] = pen.__s1.x + 1;
    cursor_bounds->n[0] = pen.__s1.y - last_font_header->ascending_height;
    cursor_bounds->n[2] = last_font_header->descending_height + pen.__s1.y;

    text_bounds->n[1] = text_bounds_globals.bounds.n[1];
    text_bounds->n[0] = bounds->n[0];
    text_bounds->n[3] = text_bounds_globals.bounds.n[3];
    text_bounds->n[2] = cursor_bounds->n[2];
}
