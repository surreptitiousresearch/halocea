#include "headers/leaf_map.h"
#include "headers/map_leaf.h"

int leaf_map_leaf_is_closed(const leaf_map *leaf_map, int leaf_index)
{
    map_leaf *leaf = &((map_leaf *)leaf_map->leaves.address)[leaf_index];
    int portal_designator_count = leaf->portal_designators.count;

    if ( portal_designator_count <= 0 )
        return (unsigned __int8)portal_designator_count;

    int *portal_designators = (int *)leaf->portal_designators.address;
    for ( int i = 0; i < portal_designator_count; i++ )
    {
        if ( portal_designators[i] >= 0 )
            return 0;
    }
    return (unsigned __int8)portal_designator_count;
}
