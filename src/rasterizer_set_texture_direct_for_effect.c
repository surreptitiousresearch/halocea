/* rasterizer_set_texture_direct_for_effect 0x8369C650 — resolve a bitmap tag group + bitmap index to a
 * concrete bitmap_data and bind it to a pixel-shader effect texture stage. Returns 1 on success, 0 if the
 * group reference is null, holds no bitmaps, or the bitmap could not be fetched. The bitmap index wraps
 * modulo the group's bitmap count. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_dx9_shader_table.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int rasterizer_set_texture_bitmap_data_for_effect(int16_t stage, bitmap_data *bitmap, rasterizer_dx9_shader *dxeffect_shader);

int rasterizer_set_texture_direct_for_effect(int16_t stage, int bitmap_group_index, int16_t bitmap_index,
        rasterizer_dx9_shader *dxeffect_shader)
{
    const bitmap_group *group;
    bitmap_data *bitmap;   /* non-const: source (bitmap_group_try_and_get_bitmap) and sink are both non-const */

    if ( bitmap_group_index == -1 )
        return 0;

    /* (*(_DWORD **)TAG_INSTANCE(idx))[24] is bitmap_group->bitmaps.count (tag_block at byte 0x60). */
    group = TAG_GET(const bitmap_group, bitmap_group_index);
    if ( group->bitmaps.count <= 0 )
        return 0;

    bitmap = bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index % group->bitmaps.count);
    if ( !bitmap )
        return 0;

    rasterizer_set_texture_bitmap_data_for_effect(stage, bitmap, dxeffect_shader);
    return 1;
}
