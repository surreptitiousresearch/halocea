/* rasterizer_set_texture_for_effect @0x8369C818 */
/* rasterizer_set_texture_for_effect 0x8369C818 — variant of rasterizer_set_texture that also passes
 * a DX9 effect shader to the bind call. Falls back to the per-type default texture on type mismatch
 * or missing group, exactly as the blocking variant does. Returns bound dimensions or null. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/point2d.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/blam_data_globals.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);


point2d *rasterizer_set_texture_for_effect(int16_t stage, int16_t type, int16_t usage,
                                           int bitmap_group_index, int16_t bitmap_index,
                                           rasterizer_dx9_shader *dxeffect_shader)
{
    char bound = 0;

    if ( (rasterizer_debug_options.bump_mapping_enabled || usage != 3) && bitmap_group_index != -1 )
    {
        const bitmap_group *group = TAG_GET(const bitmap_group, bitmap_group_index);
        if ( group->bitmaps.count > 0 )
        {
            bitmap_data *bitmap =
                bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index % group->bitmaps.count);
            if ( bitmap->type == type )
            {
                rasterizer_set_texture_bitmap_data_for_effect(stage, bitmap, dxeffect_shader);
                bound = 1;
                dimensions_0 = *(point2d *)&bitmap->width;
            }
        }
    }

    if ( !bound )
    {
        int default_index = global_rasterizer_data->default_textures[type].index;
        if ( default_index != -1 )
        {
            bitmap_data *fallback = bitmap_group_try_and_get_bitmap(default_index, usage);
            if ( fallback )
            {
                rasterizer_set_texture_bitmap_data_for_effect(stage, fallback, dxeffect_shader);
                bound = 1;
                dimensions_0 = *(point2d *)&fallback->width;
            }
        }
    }

    return bound ? &dimensions_0 : nullptr;
}
