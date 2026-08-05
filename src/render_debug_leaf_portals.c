/* render_debug_leaf_portals @0x8381B3E8 — debug visualization of all portals of one BSP leaf. As with
 * render_debug_leaf_portal the per-portal draw body is compiled out in this release build, leaving only the
 * loop over the leaf's portal count (leaf record stride 6 dwords, count at +3). Reproduced faithfully. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/map_leaf.h"

void render_debug_leaf_portals(const leaf_map *map, int leaf_index)
{
    int portal_count = ((const map_leaf *)map->leaves.address)[leaf_index].portal_designators.count;
    if (portal_count > 0)
    {
        int16_t portal = 0;
        do
            portal = (int16_t)(portal + 1);
        while (portal < portal_count);
    }
}
