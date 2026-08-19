/* render_state_text @0x836A3B58 — draw a UI state line (e.g. lobby/loading status text) inside `bounds`,
 * advancing the caller's cursor: indent by the cursor's offset into the bounds, compute the text bounds,
 * nudge the cursor up 3 pixels, top-align the text, and draw it — through the icon-substituting path when
 * `custom` is set and a game engine is running, else the plain unicode draw. The bounds' left edge is
 * advanced to the cursor column either way. */

#include <stdint.h>
#include "headers/rectangle2d.h"
#include <stddef.h>   /* wchar_t */

typedef struct point2d point2d;

extern void draw_string_set_indents(int16_t initial_indent, int16_t paragraph_indent);
extern void draw_unicode_string_compute_bounds(const rectangle2d *bounds, const wchar_t *string, rectangle2d *text_bounds, rectangle2d *cursor_bounds);
extern uint8_t game_engine_running(void);
extern void draw_string_and_hack_in_icons(rectangle2d *bounds, rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *instring, uint8_t ignore_icon_color);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);

void render_state_text(rectangle2d *bounds, rectangle2d *cursor_bounds, const wchar_t *text, uint8_t custom)
{
    draw_string_set_indents(cursor_bounds->n[1] - bounds->n[1], 0);

    rectangle2d text_bounds;
    draw_unicode_string_compute_bounds(bounds, text, &text_bounds, cursor_bounds);

    cursor_bounds->n[1] -= 3;
    text_bounds.n[1] = bounds->n[1];

    if ( custom && game_engine_running() )
        draw_string_and_hack_in_icons(&text_bounds, 0, 0, 0, text, 1u);
    else
        rasterizer_draw_unicode_string(&text_bounds, 0, 0, 0, text);

    bounds->n[0] = cursor_bounds->n[0];
}
