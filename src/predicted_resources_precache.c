/* predicted_resources_precache @0x8370E6D0 — walk a tag's predicted-resources block and precache each bitmap
 * resource into the texture cache (skipped entirely when drawing is disabled). */

#include <stdint.h>
#include "headers/tag_block.h"
#include "headers/global_tag_instances.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/predicted_resource.h"
#include "headers/predicted_resource_type.h"
#include "headers/blam_data_globals.h"


extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);

void predicted_resources_precache(const tag_block *predicted_resources)
{
    if ( debug_no_drawing || predicted_resources->count <= 0 )
        return;
    for ( int16_t i = 0; i < predicted_resources->count; i = (int16_t)(i + 1) )
    {
        predicted_resource *resource = &((predicted_resource *)predicted_resources->address)[i];
        if ( resource->type == _predicted_resource_bitmap )
        {
            bitmap_group *group = TAG_GET(bitmap_group, resource->tag_index);
            bitmap_data *bitmap =
                &((bitmap_data *)group->bitmaps.address)[resource->resource_index];
            _texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u);
        }
    }
}
