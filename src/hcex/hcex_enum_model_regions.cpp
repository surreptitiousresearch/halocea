/* hcex_enum_model_regions @0x83682DF0 — iterate every region of a Blam model tag and invoke a callback with
 * the region record. Fetches the model definition via TAG_INSTANCE(model_index), then walks its region
 * block. The loop counter is truncated to 16 bits each step, matching the tag's short region index. */

#include "../headers/global_tag_instances.h"
#include "../headers/model.h"
#include "../headers/model_region.h"

extern "C" void hcex_enum_model_regions(int model_index, void (*cb)(void *mdl, const char *region), void *mdl)
{
    if ( model_index == -1 )
        return;

    model *model_definition = *(model **)TAG_INSTANCE(model_index);
    int region_count = model_definition->regions.count;
    if ( region_count <= 0 )
        return;

    int region_index = 0;
    do
    {
        cb(mdl, (const char *)&((model_region *)model_definition->regions.address)[region_index]);
        region_index = (__int16)(region_index + 1);
    }
    while ( region_index < model_definition->regions.count );
}
