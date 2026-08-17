/* rasterizer_set_texture @0x8369C960 */
/* rasterizer_set_texture 0x8369C960 — bind a bitmap to a texture stage (blocking variant). Resolves the requested
 * bitmap from its group; if it matches the requested type it binds it and records its {width,height}. When nothing
 * is bound (bump map disabled with usage 3, missing/empty group, or type mismatch) it falls back to the
 * rasterizer's per-type default texture. Returns the bound texture's dimensions, or null when nothing was bound. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/point2d.h"
#include "headers/blam_data_globals.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap);


point2d *rasterizer_set_texture(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index,
                                int16_t bitmap_index)
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
                rasterizer_set_texture_bitmap_data(stage, bitmap);
                bound = 1;
                dimensions = *(point2d *)&bitmap->width;
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
                rasterizer_set_texture_bitmap_data(stage, fallback);
                bound = 1;
                dimensions = *(point2d *)&fallback->width;
            }
        }
    }

    return bound ? &dimensions : nullptr;
}
