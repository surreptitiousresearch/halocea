/* texture_cache_bitmap_new @0x836F3E28 — initialise a bitmap_data for streaming from the texture
 * cache: flag it cached, clear its runtime handles, resolve its absolute pixel file offset (the tag
 * group's pixel_data file offset plus the bitmap's local offset) and compute its pixel-data size. */

#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"

extern int bitmap_get_pixel_data_size(const bitmap_data *bitmap);

void texture_cache_bitmap_new(int tag_index, bitmap_data *bitmap)
{
    int local_pixels_offset = bitmap->pixels_offset;

    bitmap->base_address = 0;
    bitmap->flags |= (1u << _bitmap_cached_bit);
    bitmap->cache_block_index = -1;
    bitmap->hardware_format = 0;

    /* base_address of the tag's loaded data == the bitmap_group definition */
    bitmap_group *group = TAG_GET(bitmap_group, tag_index);
    bitmap->pixels_offset = group->pixel_data.file_offset + local_pixels_offset;

    bitmap->pixels_size = bitmap_get_pixel_data_size(bitmap);
    bitmap->tag_index = tag_index;
    bitmap->cache_block_index = -1;
    bitmap->base_address = 0;
    bitmap->hardware_format = 0;
}
