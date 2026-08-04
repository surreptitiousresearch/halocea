/* _rasterizer_environment_lightmap_begin @0x8378EF60 — begin a diffuse lightmap batch: record the lightmap
 * bitmap and, when a debug lightmap_mode is active, force the ambient color to either a flat debug value
 * (mode 2) or a random per-lightmap color (mode 1). */

#include "headers/bitmap_data.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/blam_data_globals.h"

extern float real_seed_random(uint32_t *seed);

void _rasterizer_environment_lightmap_begin(const bitmap_data *lightmap)
{
    if (!rasterizer_debug_options.draw_environment_lightmaps)
        return;

    global_lightmap = (bitmap_data *)lightmap;
    if (rasterizer_globals.lightmap_mode <= 0)
        return;

    if (rasterizer_globals.lightmap_mode == 2)
    {
        local_lightmap_ambient_color.__s1.blue = rasterizer_debug_options.lightmap_ambient;
        local_lightmap_ambient_color.__s1.green = rasterizer_debug_options.lightmap_ambient;
        local_lightmap_ambient_color.__s1.red = rasterizer_debug_options.lightmap_ambient;
    }
    else
    {
        unsigned int seed = (unsigned int)lightmap;
        local_lightmap_ambient_color.__s1.red = real_seed_random(&seed);
        local_lightmap_ambient_color.__s1.green = real_seed_random(&seed);
        local_lightmap_ambient_color.__s1.blue = real_seed_random(&seed);
    }
}
