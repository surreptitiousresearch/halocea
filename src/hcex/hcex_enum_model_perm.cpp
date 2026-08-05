/* hcex_enum_model_perm @0x83682E68 — iterate every permutation of one region of a Blam model tag, invoking a
 * callback per permutation record. Fetches the model definition via *(int *)TAG_INSTANCE(model_index),
 * validates region_index against the region count, locates the region record in the region block, then
 * walks that region's permutation block. The loop counter is truncated to 16 bits each step, matching the
 * tag's short permutation index. */

#include <stdint.h>
#include "../headers/global_tag_instances.h"
#include "../headers/model.h"
#include "../headers/model_region.h"
#include "../headers/model_region_permutation.h"

extern "C" void hcex_enum_model_perm(int model_index, int region_index,
        void (*cb)(void *mdl, int region_index, const char *permutation), void *mdl)
{
    if ( model_index == -1 )
        return;

    model *model_definition = *(model **)TAG_INSTANCE(model_index);
    if ( region_index < 0 || region_index >= model_definition->regions.count )
        return;

    model_region *region = &((model_region *)model_definition->regions.address)[region_index];
    int permutation_count = region->permutations.count;
    if ( permutation_count <= 0 )
        return;

    int permutation_index = 0;
    do
    {
        cb(mdl, region_index,
           (const char *)&((model_region_permutation *)region->permutations.address)[permutation_index]);
        permutation_index = (int16_t)(permutation_index + 1);
    }
    while ( permutation_index < region->permutations.count );
}
