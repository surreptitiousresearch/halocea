/* leaf_map_delete @0x8381A940 */
#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/tag_block.h"
#include "headers/tag_groups.h"


void leaf_map_delete(leaf_map *leaf_map)
{
    tag_block_resize(&leaf_map->leaves, 0);
    tag_block_resize(&leaf_map->portals, 0);
}
