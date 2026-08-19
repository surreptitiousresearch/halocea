/* drawline @0x83746550 — draw one line of debug/console-style text at a fixed 18px line height (26px tall
 * bounds), positioned within the camera's viewport (offset by the viewport's own origin so window_bounds'
 * y1/x1 edges become viewport-relative).
 *
 * DEVIATION: the decompiler rendered the window_bounds.y1/x1 copy and the viewport_bounds.n[0]-into-x0 copy
 * as raw DWORD/WORD punning on the rectangle2d union; reproduced as explicit per-field n[] copies (rectangle2d
 * field order is y0,x0,y1,x1 — see rectangle2d.h) rather than the punned form, since the underlying bytes are
 * identical either way. */

#include <wchar.h>
#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


#include "headers/point2d.h"
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void draw_string_set_format(int16_t style, int16_t justification, unsigned int flags);
extern void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const wchar_t *string);
/* DEVIATION: `line` is an int, not an int16_t — r4 is never sign-extended (`mr r31, r4` @0x83746560, then `slwi`/`add` @0x837465A0 for line*18). */
void drawline(const wchar_t *text, int line, int16_t justification)
{
    rectangle2d bounds;
    bounds.n[2] = render.camera.window_bounds.n[2]; /* y1 */
    bounds.n[3] = render.camera.window_bounds.n[3]; /* x1 */
    bounds.n[0] = 0;                                 /* y0 */
    bounds.n[1] = render.camera.viewport_bounds.n[0]; /* x0 <- viewport y0 */

    rectangle2d line_bounds = bounds;
    offset_rectangle2d(&line_bounds, -render.camera.viewport_bounds.n[1], -render.camera.viewport_bounds.n[0]);
    line_bounds.n[0] = 18 * line;
    line_bounds.n[2] = 18 * line + 26;

    draw_string_set_format(-1, justification, 0);
    rasterizer_draw_unicode_string(&line_bounds, 0, 0, 0, text);
}
