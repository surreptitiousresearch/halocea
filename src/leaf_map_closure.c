/* leaf_map_closure @0x8381BB18 — compute the transitive portal-connectivity closure of a set of marked
 * leaves: for every marked leaf, follow each of its portals to the leaf on the other side and mark that
 * leaf too (recursing via leaf_map_family_mark for newly-marked leaves), so the result bitset ends up
 * containing every leaf reachable from the input set through open portals. marked_leaves/result are
 * bitsets (one bit per leaf index); marked_leaves may alias result, in which case no initial copy is
 * needed. Always returns 1. */

#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/leaf_portal.h"
#include "headers/bit_vector.h"

extern void *memcpy(void *destination, const void *source, unsigned int size);
extern void leaf_map_family_mark(const leaf_map *leaf_map, unsigned int *marked_leaves, int leaf_index);

int leaf_map_closure(const leaf_map *leaf_map, unsigned int *marked_leaves, unsigned int *result)
{
    if ( marked_leaves != result )
        memcpy(result, marked_leaves, 4 * BIT_VECTOR_SIZE_IN_LONGS(leaf_map->leaves.count));

    for ( int leaf_index = 0; leaf_index < leaf_map->leaves.count; ++leaf_index )
    {
        if ( !BIT_VECTOR_TEST_FLAG(result, leaf_index) )
            continue;

        const map_leaf *leaf = &((const map_leaf *)leaf_map->leaves.address)[leaf_index];
        if ( leaf->portal_designators.count <= 0 )
            continue;

        const int *portal_designators = (const int *)leaf->portal_designators.address;
        for ( int i = 0; i < leaf->portal_designators.count; ++i )
        {
            const leaf_portal *portal = &((const leaf_portal *)leaf_map->portals.address)[portal_designators[i]];

            int other_leaf_index = portal->leaf_indices[0];
            if ( other_leaf_index == leaf_index )
                other_leaf_index = portal->leaf_indices[1];

            unsigned int word = result[other_leaf_index >> 5];
            unsigned int bit = 1u << (other_leaf_index & 0x1F);
            if ( (bit & word) == 0 )
            {
                result[other_leaf_index >> 5] = bit | word;
                leaf_map_family_mark(leaf_map, result, other_leaf_index);
            }
        }
    }

    return 1;
}
