/* leaf_map_close_portal @0x8381AE18 — mark `portal_index` closed in both leaves it connects: for each side
 * (portal record +4 and +8, a leaf index with a high "flag" bit that must be masked off), scan that leaf's
 * portal-reference list (leaf record +0xC count, +0x10 list) for an entry matching `portal_index` (also
 * masked) and OR 0x80000000 into it to flag the portal closed.
 *
 * DEVIATION: the decompiler rendered the leaf-record address as `16 * raw + 8 * (raw & 0x7FFFFFFF)` —
 * duplicating one masked register into two differently-masked terms. Disasm (0x8381AE08-0x8381AEB4) shows
 * only a single `clrlwi` (clear the top bit) whose result feeds the entire address computation via the
 * compiler's shift-add strength reduction for `*24` (masked*2, +masked, <<3) — i.e. the true formula is
 * plainly `24 * (raw & 0x7FFFFFFF)`, matching the portal record's own (correctly decompiled) `24 * portal_index`
 * stride. Restored accordingly; same sign-trick/bit-formula tautology class as convex_hull2d_test_point_indexed.c. */

#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/leaf_portal.h"

void leaf_map_close_portal(leaf_map *leaf_map, int portal_index)
{
    leaf_portal *portal = &((leaf_portal *)leaf_map->portals.address)[portal_index];

    for ( int side = 0; side < 2; ++side )
    {
        int raw_leaf_index = portal->leaf_indices[side];
        int leaf_index = raw_leaf_index & 0x7FFFFFFF;
        map_leaf *leaf = &((map_leaf *)leaf_map->leaves.address)[leaf_index];

        int portal_ref_count = leaf->portal_designators.count;
        if ( portal_ref_count <= 0 )
            continue;

        int *portal_refs = (int *)leaf->portal_designators.address;
        for ( int i = 0; i < portal_ref_count; ++i )
        {
            int raw_portal_ref = portal_refs[i];
            if ( (raw_portal_ref & 0x7FFFFFFF) == portal_index )
            {
                portal_refs[i] = raw_portal_ref | 0x80000000;
                break;
            }
        }
    }
}
