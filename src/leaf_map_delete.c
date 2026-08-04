#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/tag_block.h"

extern uint8_t tag_block_resize(tag_block *block, int element_count);

void leaf_map_delete(leaf_map *leaf_map)
{
    tag_block_resize(&leaf_map->leaves, 0);
    tag_block_resize(&leaf_map->portals, 0);
}
