/* render_state_text_0 @0x83731AF0 — draw one plain-text run at the current cursor position and advance
 * it. Same indent/bounds idiom already established in draw_string_and_hack_in_icons.c (this is in fact
 * one of that function's own callees). */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include <stddef.h>   /* wchar_t */

#include "headers/point2d.h"
extern void draw_string_set_indents(int16_t initial_indent, int16_t paragraph_indent);
extern void draw_unicode_string_compute_bounds(const rectangle2d *bounds, const wchar_t *string, rectangle2d *text_bounds, rectangle2d *cursor_bounds);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void render_state_text_0(rectangle2d *bounds, rectangle2d *cursor_bounds, const wchar_t *text)
{
    /* FAITHFUL: equivalent to the binary's carry-flag idiom for max(0, diff) */
    short diff = (short)(cursor_bounds->x0 - bounds->x0);
    short initial_indent = (diff < 0) ? 0 : diff;
    draw_string_set_indents(initial_indent, 0);

    rectangle2d text_bounds;
    draw_unicode_string_compute_bounds(bounds, text, &text_bounds, cursor_bounds);
    cursor_bounds->x0 -= 3;
    text_bounds.x0 = bounds->x0;

    rasterizer_draw_unicode_string(&text_bounds, 0, 0, 0, text);

    bounds->y0 = cursor_bounds->y0;
}
