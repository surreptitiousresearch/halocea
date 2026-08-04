/* leaf_map_initialize_from_bsp @0x8381C380 — allocate a leaf_map's leaves tag_block sized to leaf_count and,
 * if the source BSP has any nodes, build its leaf faces and portals. Returns 1 on success (no error message
 * was set during the build), 0 on failure — either the tag_block allocation itself failed (in which case an
 * error message is recorded, unless one was already pending) or a later build step set leaf_map_globals.__noop.
 *
 * DEVIATION: the decompiler's failure path literally substituted the just-assigned error-message string back
 * into the return expression (`return "couldn't allocate leaf_map leaves." == nullptr;`), which is always
 * false — restored to the equivalent plain `return 0`. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/bsp3d.h"

extern uint8_t tag_block_resize(tag_block *block, int element_count);
extern void leaf_map_build_leaf_faces(leaf_map *leaf_map, unsigned int node_index);
extern void leaf_map_build_portals(leaf_map *leaf_map, int node_index);

uint8_t leaf_map_initialize_from_bsp(leaf_map *leaf_map, const bsp3d *bsp, int leaf_count)
{
    leaf_map->bsp = bsp;
    leaf_map_globals.__noop = 0;

    if ( tag_block_resize(&leaf_map->leaves, leaf_count) )
    {
        if ( bsp->nodes.count > 0 )
        {
            leaf_map_build_leaf_faces(leaf_map, 0);
            leaf_map_build_portals(leaf_map, 0);
        }
        return leaf_map_globals.__noop == 0;
    }

    if ( !leaf_map_globals.__noop )
        leaf_map_globals.__noop = "couldn't allocate leaf_map leaves.";

    return 0;
}
