/* rasterizer_draw_string @0x837693A0 — draw a single-line ASCII debug string through the dynamic
 * screen-geometry text path. Active only when the draw_dynamic_screen_geometry debug option is set and
 * the current rasterizer target is the main screen (1). The hardware character cache supplies the glyph
 * atlas bitmap; each call bumps the cache's magic_number (cache-generation stamp). The string's bounding
 * rectangle defaults to the camera window (origin-relative) and the clip rectangle to the camera viewport;
 * a caller-supplied clip is clamped into the viewport and to non-negative origin. A single-map geometry
 * parameter block is built for the atlas bitmap and the actual layout/emission is delegated to draw_string
 * with the drop-shadow glyph callback.
 *
 * DEVIATIONS (verified against disassembly):
 *   - The decompiler rendered the parameter-block fill as `*(_QWORD *)v14 = 0x1100000000LL` with a count
 *     of 17: it conflated the loop count (li r9,0x11 == 17) into the stored value. The stored register is
 *     r29 == 0 (li r29,0), so the loop is a plain zero-init of the block (17 qwords + 1 trailing dword ==
 *     sizeof params); restored as an explicit zero-init.
 *   - The 1.0f/(float)width and 1.0f/(float)height texture scales appear as std/lfd/fcfid/frsp int->float
 *     shuffles; restored as plain casts.
 *   - `__CFADD__(clip->__s1.x0, 0x80000000) ? 0 : clip->__s1.x0` is a clamp of the (sign-extended) coordinate to a
 *     non-negative value; restored as `< 0 ? 0 : value`. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/rectangle2d.h"
#include "headers/point2d.h"
#include "headers/font_header.h"
#include "headers/render_globals.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/hardware_character_cache.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/shader_framebuffer_blend_function.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"

struct font_character;
struct parse_string_state;


#include "headers/parse_string_state.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
#include "headers/font_header.h"
#include "headers/font_character.h"
extern void set_rectangle2d(rectangle2d *rectangle, int16_t x0, int16_t y0, int16_t x1, int16_t y1);
extern rectangle2d *offset_rectangle2d(rectangle2d *rectangle, int16_t dx, int16_t dy);
extern void rasterizer_text_begin(const rasterizer_dynamic_screen_geometry_parameters *parameters);
extern void rasterizer_text_end(void);
extern void rasterizer_draw_character_with_dropshadow(struct parse_string_state *parse_state, font_header *font, font_character *character, unsigned int color, int16_t x0, int16_t y0, int16_t x, int16_t y, int16_t dx, int16_t dy);
extern void draw_string(void (*draw_character)(parse_string_state *, font_header *, font_character *, unsigned int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t), const rectangle2d *bounds, point2d *cursor_reference, const rectangle2d *clip, int16_t height_adjust, const char *string);

void rasterizer_draw_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference,
                            int16_t height_adjust, const char *string)
{
    if (!rasterizer_debug_options.draw_dynamic_screen_geometry)
        return;
    if (global_window_parameters.rasterizer_target != _rasterizer_target_render_primary)
        return;

    bitmap_data *bitmap = hardware_character_cache.bitmap;
    ++magic_number;
    if (!bitmap)
        return;
    if (!*string)
        return;

    /* string bounding rectangle: caller-supplied, else the camera window made origin-relative */
    rectangle2d window_bounds;
    if (bounds)
    {
        window_bounds = *bounds;
    }
    else
    {
        window_bounds = render.camera.window_bounds;
        offset_rectangle2d(&window_bounds, -render.camera.viewport_bounds.__s1.x0, -render.camera.window_bounds.__s1.y0);
    }

    /* clip rectangle: caller-supplied (clamped into the viewport and to a non-negative origin), else the
     * camera viewport made origin-relative */
    rectangle2d viewport_bounds;
    if (clip)
    {
        __int16 clip_bottom = clip->__s1.y1;
        __int16 viewport_height = render.camera.viewport_bounds.__s1.y1 - render.camera.viewport_bounds.__s1.y0;
        if (viewport_height <= clip_bottom)
            clip_bottom = viewport_height;

        __int16 clip_right = clip->__s1.x1;
        __int16 viewport_width = render.camera.viewport_bounds.__s1.x1 - render.camera.viewport_bounds.__s1.x0;
        if (viewport_width <= clip_right)
            clip_right = viewport_width;

        set_rectangle2d(&viewport_bounds,
                        clip->__s1.x0 < 0 ? 0 : clip->__s1.x0,
                        clip->__s1.y0 < 0 ? 0 : clip->__s1.y0,
                        clip_right, clip_bottom);
    }
    else
    {
        viewport_bounds = render.camera.viewport_bounds;
        offset_rectangle2d(&viewport_bounds, -render.camera.viewport_bounds.__s1.x0, -render.camera.viewport_bounds.__s1.y0);
    }

    /* single-map geometry parameters for the glyph atlas bitmap: identity scales, no blend, bilinear */
    rasterizer_dynamic_screen_geometry_parameters parameters;
    for (unsigned char *p = (unsigned char *)&parameters;
         p < (unsigned char *)&parameters + sizeof(parameters); ++p)
        *p = 0;  /* zero-init the whole block (original: 17 qword stores + 1 dword, value == 0) */

    parameters.map_texture_scale[0].n[0] = 1.0f / (float)bitmap->width;
    parameters.map_texture_scale[0].n[1] = 1.0f / (float)bitmap->height;
    parameters.map_scale[0].n[0] = 1.0f;
    parameters.map_scale[0].n[1] = 1.0f;
    parameters.meter_parameters = nullptr;
    parameters.point_sampled = 0;
    parameters.framebuffer_blend_function = _shader_framebuffer_blend_function_alpha_blend;
    parameters.map[0] = bitmap;

    rasterizer_text_begin(&parameters);
    draw_string(rasterizer_draw_character_with_dropshadow, &window_bounds, cursor_reference, &viewport_bounds,
                height_adjust, string);
    rasterizer_text_end();
}
