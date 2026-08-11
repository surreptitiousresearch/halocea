/* object_permute_region @0x836EC3B8 — sets the active permutation (by name) for one or all of an object's
 * model regions. For each region matching desired_region_index (-1 = all), it looks up the named
 * permutation in the region's permutation list and stores its index in the object's per-region
 * permutation byte (region_permutations[region]). When active_flag is 0 the region is reset to permutation 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/model_region_permutation.h"
#include "headers/blam_data_globals.h"

extern int stricmp(const char *a, const char *b);

void object_permute_region(int object_index, const char *permutation_name,
                           int16_t desired_region_index, uint8_t active_flag)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);
    int model_tag_index = definition->model.index;
    if (model_tag_index == -1)
        return;

    model *model_definition = TAG_GET(model, model_tag_index);
    int region_count = model_definition->regions.count;
    if (region_count <= 0)
        return;

    for (int region_index = 0; region_index < region_count; region_index = (int16_t)(region_index + 1))
    {
        if (desired_region_index != -1 && desired_region_index != region_index)
            continue;

        model_region *region = &((model_region *)model_definition->regions.address)[region_index];
        int permutation_count = region->permutations.count;
        if (permutation_count <= 0)
            continue;

        model_region_permutation *permutations =
            (model_region_permutation *)region->permutations.address;
        for (int permutation_index = 0; ; permutation_index = (int16_t)(permutation_index + 1))
        {
            if (!stricmp(permutations[permutation_index].name, permutation_name))
            {
                object->object.region_permutations[region_index] =
                    active_flag == 0 ? 0 : (uint8_t)permutation_index;
                break;
            }
            if ((int16_t)(permutation_index + 1) >= permutation_count)
                break;
        }
    }
}
