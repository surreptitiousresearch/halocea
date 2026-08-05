/* object_determine_variant_number @0x836ED3D8 — pick a model variant number for an object.
 *
 * Walks the model's regions; for each region it reads the object's currently selected permutation
 * (object.region_permutations[region]) and, if that permutation index is in range, reads the
 * permutation's variant_number. It returns the first non-zero variant number found (regions are
 * scanned in order and the loop stops as soon as a non-zero variant is picked up). */

#include <stdint.h>
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/model_region_permutation.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"

int object_determine_variant_number(int object_index, model *model)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    int region_count = model->regions.count;
    int16_t variant_number = 0;

    for ( int16_t region_index = 0; region_index < region_count;
          region_index = (int16_t)(region_index + 1) )
    {
        if ( variant_number )
            break;

        model_region *region = (model_region *)model->regions.address + region_index;
        unsigned char permutation_index = object->object.region_permutations[region_index];
        if ( permutation_index < (unsigned int)region->permutations.count )
            variant_number =
                ((model_region_permutation *)region->permutations.address)[permutation_index].variant_number;
    }

    return variant_number;
}
