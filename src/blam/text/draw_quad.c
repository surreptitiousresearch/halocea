/* draw_quad @0x83691038 — fill an axis-aligned screen rectangle with a single solid color, using the
 * rasterizer's dynamic screen-geometry path and the engine's default (white) bitmap. The rectangle's
 * four signed-int corners are converted to floats and emitted as a 4-vertex quad (clockwise from the
 * top-left), all at z=0 with zero texture coordinates and the same packed ARGB color. The default
 * bitmap (index from the rasterizer-data tag) supplies a flat texture so the quad renders as a tint.
 *
 * DEVIATION: the DB prototype is draw_quad(rectangle2d *rect, unsigned int color); the decompiler fuses
 * r3 (rect) and r4 (color) into a single 64-bit "color" (rect in HIDWORD), inventing bogus reads off
 * HIDWORD(color). Restored from the disassembly: rect corners are read as y0@0, x0@2, y1@4, x1@6. The
 * decompiler also mis-shows the params-struct zeroing loop as per-vertex texcoord stores; here the
 * parameters are zero-initialized then the named fields are set, matching the bulk memset. */

#include <string.h>
#include "headers/game_globals_tag.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_globals.h"
#include "headers/rectangle2d.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/blam_data_globals.h"
#include "headers/rasterizer_lock_operation.h"


extern void rasterizer_psuedo_dynamic_screen_quad_draw(const rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void draw_quad(rectangle2d *rect, unsigned int color)
{
    game_globals_rasterizer_data *rasterizer_data = global_game_globals->rasterizer_data.count
                               ? (game_globals_rasterizer_data *)global_game_globals->rasterizer_data.address
                               : nullptr;
    unsigned int bitmap_tag_index = (unsigned int)rasterizer_data->default_textures[0].index;

    bitmap_group *bitmap_definition = TAG_GET(bitmap_group, bitmap_tag_index);
    bitmap_data *map = (bitmap_data *)bitmap_definition->bitmaps.address + 1;

    rasterizer_globals.current_lock_operation = _rasterizer_lock_cinematics;

    /* Build the quad: corners are (x0,y0),(x1,y0),(x1,y1),(x0,y1). */
    float x0 = (float)rect->__s1.x0;
    float y0 = (float)rect->__s1.y0;
    float x1 = (float)rect->__s1.x1;
    float y1 = (float)rect->__s1.y1;

    dynamic_screen_vertex vertices[4];
    const float corner_x[4] = { x0, x1, x1, x0 };
    const float corner_y[4] = { y0, y0, y1, y1 };
    for (int i = 0; i < 4; i++)
    {
        vertices[i].position.__s1.x = corner_x[i];
        vertices[i].position.__s1.y = corner_y[i];
        vertices[i].position.__s1.z = 0.0f;
        vertices[i].color = color;
        vertices[i].texcoord.n[0] = 0.0f;
        vertices[i].texcoord.n[1] = 0.0f;
    }

    rasterizer_dynamic_screen_geometry_parameters parameters;
    memset(&parameters, 0, sizeof(parameters));
    parameters.map[0] = map;
    parameters.framebuffer_blend_function = 0;
    parameters.meter_parameters = nullptr;
    parameters.point_sampled = 0;
    parameters.map_texture_scale[0].n[1] = 1.0f;
    parameters.map_texture_scale[0].n[0] = 1.0f;
    parameters.map_scale[0].n[1] = 1.0f;
    parameters.map_scale[0].n[0] = 1.0f;

    rasterizer_psuedo_dynamic_screen_quad_draw(&parameters, vertices);

    rasterizer_globals.current_lock_operation = 0;
}
