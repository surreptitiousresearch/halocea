/* rasterizer_set_texture_non_blocking 0x8369CAC4 — bind a bitmap to a texture stage without blocking on
 * streaming, with a type check and a default-texture fallback. Resolves the requested bitmap; if its hardware
 * texture is not yet resident it returns 1 (the "would block" signal) and still marks the stage handled. If the
 * texture is resident and its type matches the requested type, it binds it and records the {width,height}. When
 * nothing was bound (disabled bump map with usage 3, missing/empty group, type mismatch) it falls back to the
 * rasterizer's per-type default texture. Returns 1 only when a real bitmap was found but not yet streamed in. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap);

extern int16_t rasterizer_last_bound_texture_height;    /* 0x844209A2 — last-bound texture height */

int rasterizer_set_texture_non_blocking(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index)
{
    char bound = 0;
    int not_ready = 0;

    if ( (rasterizer_debug_options.bump_mapping_enabled || usage != 3) && bitmap_group_index != -1 )
    {
        const bitmap_group *group = TAG_GET(const bitmap_group, bitmap_group_index);
        if ( group->bitmaps.count > 0 )
        {
            bitmap_data *bitmap =
                bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index % group->bitmaps.count);
            if ( !_texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u) )
            {
                not_ready = 1;
                bound = 1;
            }
            else if ( bitmap->type == type )
            {
                rasterizer_set_texture_bitmap_data(stage, bitmap);
                dimensions_1.__s1.x = bitmap->width; /* dimensions_1 is point2d; width is the x component */
                rasterizer_last_bound_texture_height = bitmap->height;
                bound = 1;
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
                dimensions_1.__s1.x = fallback->width; /* dimensions_1 is point2d; width is the x component */
                rasterizer_last_bound_texture_height = fallback->height;
            }
        }
    }

    return not_ready;
}
