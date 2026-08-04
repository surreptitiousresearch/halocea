/* _rasterizer_environment_reflection_lightmap_mask_begin @0x837A6710 — begin a reflection lightmap-mask
 * batch: when all the relevant debug toggles are enabled and a DX effect with a loaded shader is bound, set
 * the mask texture from the lightmap bitmap (or flag that this batch has no lightmap data). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/blam_data_globals.h"

typedef struct rasterizer_dx9_shader rasterizer_dx9_shader;
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);

void _rasterizer_environment_reflection_lightmap_mask_begin(const bitmap_data *lightmap)
{
    if (rasterizer_debug_options.drawing_mode
        || !rasterizer_debug_options.draw_environment_reflection_lightmap_masks
        || !rasterizer_debug_options.draw_environment_reflections
        || rasterizer_globals.lightmap_mode
        || !dxeffect_shader
        || !dxeffect_shader->effect)
        return;

    if (lightmap)
    {
        rasterizer_set_texture_bitmap_data_for_effect(0, (bitmap_data *)lightmap, dxeffect_shader); /* lazy hw-format mutation: drop const view */
        local_lightmap_has_no_data_0 = 0;
    }
    else
    {
        local_lightmap_has_no_data_0 = 1;
    }
}
