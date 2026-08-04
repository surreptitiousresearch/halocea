/* object_find_region_permutations_available_with_variant @0x836ED230 — collect the indices of a model
 * region's permutations that are randomly selectable (flags bit 0x1 clear) and match the requested
 * variant. A variant_number of -1 matches any real variant (variant_number < 100). Fills
 * available_indices[] with the matching permutation indices and returns the count. */

#include <stdint.h>
#include "headers/model_region.h"
#include "headers/model_region_permutation.h"
#include "headers/model_region_permutation_flags.h"

int16_t object_find_region_permutations_available_with_variant(const model_region *region, int16_t variant_number,
                                                               int16_t *available_indices)
{
    int16_t found = 0;
    if ( region->permutations.count <= 0 )
        return found;

    int16_t index = 0;
    do
    {
        /* recovered: (char *)address + 88 * index -> typed permutation[index] */
        model_region_permutation *permutation =
            (model_region_permutation *)region->permutations.address + index;
        if ( (permutation->flags & (1u << _model_region_permutation_cannot_be_chosen_randomly_bit)) == 0 )
        {
            int16_t perm_variant = permutation->variant_number;
            if ( perm_variant == variant_number || (variant_number == -1 && perm_variant < 100) )
                available_indices[found++] = index;
        }
        ++index;
    }
    while ( index < region->permutations.count );

    return found;
}
