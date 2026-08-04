/* random_item @0x83746E70 — pick a random item definition from an item collection, weighted by each
 * permutation's spawn weight. Draws a random value in [0, total weight), then walks the permutations
 * subtracting each weight until the running value goes negative; that permutation's item is returned.
 * Returns -1 if the collection is empty or the weights do not cover the drawn value. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/item_collection_definition.h"
#include "headers/item_permutation_definition.h"

extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern int16_t item_collection_get_total(const item_collection_definition *item_collection);

int random_item(int item_collection_index)
{
    item_collection_definition *collection = TAG_GET(item_collection_definition, item_collection_index);
    int count = collection->permutations.count;
    __int16 total_weight = item_collection_get_total(collection);
    int running = seed_random_range(get_global_random_seed_address(), 0, total_weight);

    if (count <= 0)
        return -1;

    item_permutation_definition *permutations = (item_permutation_definition *)collection->permutations.address;
    int index = 0;
    while (1)
    {
        running = (int)((float)running - permutations[index].weight);
        if (running < 0)
            break;
        if (++index >= count)
            return -1;
    }
    return permutations[index].item.index;
}
