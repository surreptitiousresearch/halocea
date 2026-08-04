/* rasterizer_set_texture_direct @0x8369C6E0 — bind a bitmap out of a bitmap-group tag to a
 * fixed-function sampler stage. The bitmap index wraps modulo the group's bitmap count so callers
 * can pass a free-running counter. Returns 0 if the group tag index is unset, the group has no
 * bitmaps, or the (wrapped) bitmap could not be fetched. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"

extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int rasterizer_set_texture_bitmap_data(int16_t stage, bitmap_data *bitmap);

int rasterizer_set_texture_direct(int16_t stage, int bitmap_group_index, int16_t bitmap_index)
{
    if (bitmap_group_index == -1)
        return 0;

    bitmap_group *group = TAG_GET(bitmap_group, bitmap_group_index);
    int bitmap_count = group->bitmaps.count;
    if (bitmap_count <= 0)
        return 0;

    const bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index % bitmap_count);
    if (!bitmap)
        return 0;

    rasterizer_set_texture_bitmap_data(stage, (bitmap_data *)bitmap); /* lazy hw-format mutation: drop const view */
    return 1;
}
