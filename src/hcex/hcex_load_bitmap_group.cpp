/* hcex_load_bitmap_group @0x83707C98 — force every bitmap in a bitmap_group tag to upload/realize its
 * hardware texture format. */

#include <stdint.h>
#include "../headers/global_tag_instances.h"
#include "../headers/bitmap_group.h"
#include "../headers/bitmap_data.h"

extern "C" int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);

extern "C" void hcex_load_bitmap_group(int bitmap_group_index)
{
    if ( bitmap_group_index != -1 )
    {
        bitmap_group *group = *(bitmap_group **)TAG_INSTANCE(bitmap_group_index);

        for ( int i = 0; i < group->bitmaps.count; ++i )
        {
            bitmap_data *bitmap = &((bitmap_data *)group->bitmaps.address)[i];

            if ( bitmap )
                _texture_cache_bitmap_get_hardware_format(bitmap, 1u, 1u);
        }
    }
}
