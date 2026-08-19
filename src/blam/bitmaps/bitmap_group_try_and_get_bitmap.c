/* bitmap_group_try_and_get_bitmap @0x837940D8 — fetch the bitmap_data for a given bitmap index out of
 * a bitmap-group tag, range-checking the index against the group's bitmap count. Returns null if the
 * group tag is absent or the index is out of range. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"

bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index)
{
    bitmap_group *group = TAG_GET(bitmap_group, bitmap_group_index);

    if ( group && bitmap_index >= 0 && bitmap_index < group->bitmaps.count )
        return &((bitmap_data *)group->bitmaps.address)[bitmap_index];
    return nullptr;
}
