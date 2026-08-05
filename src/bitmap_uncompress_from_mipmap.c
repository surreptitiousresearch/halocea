/* bitmap_uncompress_from_mipmap @0x8377E150 — dispatches mipmap decompression by source bitmap type
 * (0=2D, 1=3D, 2=cubemap). */

#include <stdint.h>
#include "headers/bitmap_data.h"
#include "headers/bitmap_type.h"

extern void bitmap_2d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index);
extern void bitmap_3d_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index);
extern void bitmap_cm_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index);

void bitmap_uncompress_from_mipmap(const bitmap_data *source_bitmap, bitmap_data *destination_bitmap, int16_t source_mipmap_index)
{
    if (!source_bitmap->type)
        bitmap_2d_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
    else if (source_bitmap->type == bitmap_type_3d)
        bitmap_3d_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
    else if ((uint16_t)source_bitmap->type < bitmap_type_white)
        bitmap_cm_uncompress_from_mipmap(source_bitmap, destination_bitmap, source_mipmap_index);
}
