/* _rasterizer_environment_specular_lightmap_begin @0x837A5CA8 — begin a specular lightmap batch: when not in
 * a special drawing mode and specular lightmaps are enabled, record the lightmap bitmap (or flag that this
 * batch has no lightmap data). */

#include "headers/bitmap_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"

extern bitmap_data *global_specular_lightmap;   /* DB: global_specular_lightmap */

void _rasterizer_environment_specular_lightmap_begin(const bitmap_data *lightmap)
{
    if (rasterizer_debug_options.drawing_mode || !rasterizer_debug_options.draw_environment_specular_lightmaps)
        return;

    if (lightmap)
    {
        local_lightmap_has_no_data = 0;
        global_specular_lightmap = (bitmap_data *)lightmap;
    }
    else
    {
        local_lightmap_has_no_data = 1;
        global_specular_lightmap = 0;
    }
}
