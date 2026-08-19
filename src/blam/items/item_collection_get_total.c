/* item_collection_get_total @0x83746D30 — sum the spawn weights of every permutation in an item collection.
 * The running total is kept in an integer (each addition is rounded toward zero), matching the original
 * `long total += permutation->weight` accumulation.
 *
 * DEVIATION: the decompiler unrolled the loop by four and rendered each int<->float conversion as
 * std/lfd/fcfid/fctiwz shuffles; restored to a single weight-summing loop. */

#include <stdint.h>
#include "headers/item_collection_definition.h"
#include "headers/item_permutation_definition.h"

int16_t item_collection_get_total(const item_collection_definition *item_collection)
{
    int total = 0;
    const item_permutation_definition *permutations = (const item_permutation_definition *)item_collection->permutations.address;

    for (int i = 0; i < item_collection->permutations.count; ++i)
        total += permutations[i].weight;

    /* truncation models the caller-side extsh (random_item @0x83746E7C) */
    return (int16_t)total;
}
