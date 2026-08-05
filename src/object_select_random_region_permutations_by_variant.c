/* object_select_random_region_permutations_by_variant @0x836ED2C0 — for each region of an object's
 * model, choose a permutation matching the requested variant (falling back to variant 0 when the
 * requested variant has none), pick one at random when several qualify, and store the chosen
 * permutation byte at object_data->object.region_permutations[region_index]. Returns 1 if every region
 * resolved a permutation,
 * 0 if any region had none (that region's byte is set to 0). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/model.h"
#include "headers/model_region.h"
#include "headers/blam_data_globals.h"


extern int16_t object_find_region_permutations_available_with_variant(const model_region *region, int16_t variant_number, int16_t *available_indices);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

uint8_t object_select_random_region_permutations_by_variant(int object_index, const model *model, int16_t variant_number)
{
    int all_resolved = 1;
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( model->regions.count <= 0 )
        return all_resolved;

    int16_t available_permutations[72];
    int region_index = 0;
    do
    {
        model_region *region = &((model_region *)model->regions.address)[region_index];
        int16_t count = object_find_region_permutations_available_with_variant(
            region, variant_number, available_permutations);
        if ( !count && variant_number != -1 )
            count = object_find_region_permutations_available_with_variant(region, 0, available_permutations);

        if ( count )
        {
            int16_t pick;
            if ( count == 1 )
                pick = 0;
            else
                pick = seed_random_range(get_global_random_seed_address(), 0, count);
            object_data->object.region_permutations[region_index] = (unsigned char)available_permutations[pick];
        }
        else
        {
            all_resolved = 0;
            object_data->object.region_permutations[region_index] = 0;
        }
        region_index = (int16_t)(region_index + 1);
    }
    while ( region_index < model->regions.count );

    return all_resolved;
}
