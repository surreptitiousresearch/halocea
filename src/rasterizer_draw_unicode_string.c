/* rasterizer_draw_unicode_string @0x83769600 — draw a unicode string through the dynamic screen-geometry
 * path using the hardware font glyph cache. Resolves the on-screen bounds rectangle (defaulting to the
 * camera window) and the clip rectangle (defaulting to the camera viewport, otherwise the caller's clip
 * intersected with the viewport), configures the geometry parameters with the glyph-cache bitmap, then
 * walks the string with draw_unicode_string using the drop-shadow character renderer.
 *
 * Only runs when dynamic screen geometry drawing is enabled, the target is the main framebuffer
 * (rasterizer_target == _rasterizer_target_render_primary), the glyph cache has a bitmap, and the string is non-empty.
 *
 * DEVIATION: the decompiler rendered the parameter-struct zeroing loop as repeated 0x1100000000 QWORD
 * stores (the 0x11 is the loop count of 17, the stored value is 0) and the texture-scale reciprocals via
 * fcfid int<->float shuffles; restored to a memset plus plain float divisions. */

#include <wchar.h>
#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/bitmap_data.h"
#include "headers/font_header.h"
#include "headers/hardware_character_cache.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/blam_data_globals.h"


#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
extern void hcex_draw_string(const wchar_t *string);
extern unsigned int ustrlen(const wchar_t *string);
extern void set_rectangle2d(rectangle2d *rectangle, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void rasterizer_text_begin(const rasterizer_dynamic_screen_geometry_parameters *parameters);
extern void rasterizer_text_end(void);
extern void rasterizer_draw_character_with_dropshadow(struct parse_string_state *parse_state, font_header *font, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy);
extern void draw_unicode_string(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust, const wchar_t *string);
/* memset provided by CRT via <wchar.h> chain (propagate auto-inserted); local non-standard extern removed to avoid C4028 (size_t vs unsigned int) */

void rasterizer_draw_unicode_string(const rectangle2d *bounds, const rectangle2d *clip,
                                    point2d *cursor_reference, int16_t height_adjust, const wchar_t *string)
{
    hcex_draw_string(string);

    if (!rasterizer_debug_options.draw_dynamic_screen_geometry)
        return;
    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
        return;

    bitmap_data *bitmap = hardware_character_cache.bitmap;
    ++magic_number;
    if (!bitmap || !*string)
        return;

    ustrlen(string);

    rectangle2d window_bounds;
    if (bounds)
    {
        window_bounds = *bounds;
    }
    else
    {
        window_bounds = render.camera.window_bounds;
        offset_rectangle2d(&window_bounds, -render.camera.viewport_bounds.n[1], -render.camera.window_bounds.n[0]);
    }

    rectangle2d viewport_bounds;
    if (clip)
    {
        int16_t viewport_height = render.camera.viewport_bounds.n[2] - render.camera.viewport_bounds.n[0];
        int16_t viewport_width = render.camera.viewport_bounds.n[3] - render.camera.viewport_bounds.n[1];
        int16_t y1 = (viewport_height <= clip->n[2]) ? viewport_height : clip->n[2];
        int16_t x1 = (viewport_width <= clip->n[3]) ? viewport_width : clip->n[3];
        set_rectangle2d(&viewport_bounds,
                        clip->n[1] < 0 ? 0 : clip->n[1],
                        clip->n[0] < 0 ? 0 : clip->n[0],
                        x1, y1);
    }
    else
    {
        viewport_bounds = render.camera.viewport_bounds;
        offset_rectangle2d(&viewport_bounds, -render.camera.viewport_bounds.n[1], -render.camera.viewport_bounds.n[0]);
    }

    rasterizer_dynamic_screen_geometry_parameters parameters;
    memset(&parameters, 0, sizeof(parameters));
    parameters.map_texture_scale[0].n[0] = 1.0f / (float)bitmap->width;
    parameters.map_texture_scale[0].n[1] = 1.0f / (float)bitmap->height;
    parameters.map_scale[0].n[0] = 1.0f;
    parameters.map_scale[0].n[1] = 1.0f;
    parameters.meter_parameters = nullptr;
    parameters.point_sampled = 0;
    parameters.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_blend; /* DB _shader_framebuffer_blend_function enum, value 0 */
    parameters.map[0] = bitmap;

    rasterizer_text_begin(&parameters);
    draw_unicode_string(rasterizer_draw_character_with_dropshadow, &window_bounds, cursor_reference,
                        &viewport_bounds, height_adjust, string);
    rasterizer_text_end();
}
