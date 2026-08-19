/* render_debug_leaf_portal @0x8381B3A8 — debug visualization of one BSP leaf portal's edges. In this release
 * build the per-edge draw body is compiled out, leaving only the loop that reads the portal's vertex count
 * (portal record stride 6 dwords, count at +3) and walks its edges from vertex 2. Reproduced faithfully. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/leaf_portal.h"

void render_debug_leaf_portal(const leaf_map *map, int portal_designator)
{
    /* DEVIATION: the portal offset is 8*(2*designator + (designator & 0x7FFFFFFF)) — slwi r11,r4,1 /
     * clrlwi r10,r4,1 / add / slwi 3 @0x8381B3A8-0x8381B3B8: the flag bit is masked from only one
     * addend, so a flagged designator does NOT reduce to a plain stride-24 subscript. */
    int vertex_count = ((const leaf_portal *)((const char *)map->portals.address
                        + 8 * (2 * portal_designator + (portal_designator & 0x7FFFFFFF))))->vertices.count;
    if (vertex_count > 2)
    {
        int16_t vertex = 2;
        do
            vertex = (int16_t)(vertex + 1);
        while (vertex < vertex_count);
    }
}
