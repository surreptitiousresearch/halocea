/* unit_find_dialogue_variant @0x837BAA90 — pick a dialogue variant for a unit. Scans the unit definition's
 * dialogue_variants block (tag_block @ +0x2B4 count / +0x2B8 address, 24-byte entries: variant id @ +0,
 * dialogue tag index @ +20). Collects all entries matching variant_number (or all when -1), then returns
 * one at random (or the single match). Returns the dialogue tag index, or -1 if none match. */

#include <stdint.h>
#include "headers/unit_definition.h"
#include "headers/dialogue_variant_definition.h"

extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern uint32_t *get_global_random_seed_address(void);

int unit_find_dialogue_variant(const unit_definition *definition, int16_t variant_number)
{
    int count = definition->unit.dialogue_variants.count;
    dialogue_variant_definition *variants = (dialogue_variant_definition *)definition->unit.dialogue_variants.address;

    int16_t matches[20];
    int match_count = 0;
    if ( count > 0 )
    {
        int index = 0;
        do
        {
            if ( variant_number == -1 || variants[index].variant_number == variant_number )
                matches[match_count++] = index;
            index = (int16_t)(index + 1);
        }
        while ( index < count );
    }

    if ( match_count > 0 )
    {
        int16_t chosen;
        if ( match_count == 1 )
        {
            chosen = matches[0];
        }
        else
        {
            unsigned int *seed = get_global_random_seed_address();
            chosen = matches[seed_random_range(seed, 0, match_count)];
        }
        return variants[chosen].dialogue_variant.index;
    }
    return -1;
}
