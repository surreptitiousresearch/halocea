/* render_debug_leaf_portal @0x8381B3A8 — debug visualization of one BSP leaf portal's edges. In this release
 * build the per-edge draw body is compiled out, leaving only the loop that reads the portal's vertex count
 * (portal record stride 6 dwords, count at +3) and walks its edges from vertex 2. Reproduced faithfully. */

#include "headers/leaf_map.h"
#include "headers/leaf_portal.h"

void render_debug_leaf_portal(const leaf_map *map, int portal_designator)
{
    int vertex_count = ((const leaf_portal *)map->portals.address)[portal_designator].vertices.count;
    if (vertex_count > 2)
    {
        __int16 vertex = 2;
        do
            vertex = (__int16)(vertex + 1);
        while (vertex < vertex_count);
    }
}
