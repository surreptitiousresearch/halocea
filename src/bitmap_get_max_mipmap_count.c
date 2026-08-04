/* bitmap_get_max_mipmap_count @ 0x836F4880
   Returns the maximum possible mipmap level count for a bitmap: 0 unless the
   bitmap has power-of-two dimensions (only such bitmaps can mipmap), in which
   case it is floor_log2 of the largest of the bitmap's width/height/depth
   dimensions. */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_flags.h"

extern int16_t floor_log2(unsigned int k);

int bitmap_get_max_mipmap_count(const bitmap_data *bitmap)
{
    int largest_dimension;

    if ( (bitmap->flags & (1u << _bitmap_has_power_of_two_dimensions_bit)) == 0 )
        return 0;

    largest_dimension = bitmap->height;
    if ( bitmap->height <= bitmap->depth )
        largest_dimension = bitmap->depth;
    if ( bitmap->width > largest_dimension )
        largest_dimension = bitmap->width;

    return floor_log2(largest_dimension);
}
