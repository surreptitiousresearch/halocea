/* rasterizer_set_texture_direct_non_blocking 0x8369C790 — bind a bitmap-group bitmap to a texture stage without
 * blocking on streaming. Resolves the bitmap (wrapping the index by the group's bitmap count), and if its
 * hardware texture is already resident, binds it and returns 0; if the texture is not yet available it returns 1
 * (the "would block" signal) so the caller can defer. Returns 0 for a missing group / empty group / missing
 * bitmap as well. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap);

uint8_t rasterizer_set_texture_direct_non_blocking(int16_t stage, int bitmap_group_index, int16_t bitmap_index)
{
    if ( bitmap_group_index == -1 )
        return 0;
    const bitmap_group *group = TAG_GET(const bitmap_group, bitmap_group_index);
    if ( group->bitmaps.count <= 0 )
        return 0;

    bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index % group->bitmaps.count);
    if ( !bitmap )
        return 0;

    if ( _texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u) )
    {
        rasterizer_set_texture_bitmap_data(stage, bitmap);
        return 0;
    }
    return 1;
}
