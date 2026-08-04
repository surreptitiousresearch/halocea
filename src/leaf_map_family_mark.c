/* leaf_map_family_mark @0x8381AF20 — recursively flood-mark every leaf reachable from `leaf_index` in a
 * leaf map's portal graph into the marked_leaves bit vector (24-byte leaf and portal records; a leaf's
 * portal count/list at dwords +12/+16, a portal's two leaves at dwords +4/+8). */

#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/leaf_portal.h"

void leaf_map_family_mark(const leaf_map *leaf_map, unsigned int *marked_leaves, int leaf_index)
{
    map_leaf *leaf = &((map_leaf *)leaf_map->leaves.address)[leaf_index];

    for ( __int16 i = 0; i < leaf->portal_designators.count; i++ )
    {
        int portal_index = ((int *)leaf->portal_designators.address)[i];
        leaf_portal *portal = &((leaf_portal *)leaf_map->portals.address)[portal_index];

        int neighbor_leaf = portal->leaf_indices[0];
        if ( neighbor_leaf == leaf_index )
            neighbor_leaf = portal->leaf_indices[1];

        unsigned int word_index = neighbor_leaf >> 5;
        unsigned int bit = 1 << (neighbor_leaf & 0x1F);
        if ( (marked_leaves[word_index] & bit) == 0 )
        {
            marked_leaves[word_index] |= bit;
            leaf_map_family_mark(leaf_map, marked_leaves, neighbor_leaf);
        }
    }
}
