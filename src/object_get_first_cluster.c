/* object_get_first_cluster @0x836EEFF8 — seed a cluster iterator with the structure-BSP clusters straddled by an
 * object (resolved to its ultimate parent). Picks the collideable or non-collideable cluster partition by the
 * object's flags (bit 0x2000000), records it in the iterator, and returns the first cluster
 * index (-1 if none). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_cluster_iterator.h"
#include "headers/cluster_partition.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int object_get_ultimate_parent(int object_index);
extern int16_t cluster_partition_get_first_cluster(const cluster_partition *partition, int *reference_index, int first_cluster_reference);

int16_t object_get_first_cluster(object_cluster_iterator *iterator, int object_index)
{
    int ultimate_parent = object_get_ultimate_parent(object_index);
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum;

    cluster_partition *partition;
    if ( (object->object.flags & (1u << _object_has_collision_model_bit)) != 0 )
        partition = &collideable_object_cluster_partition;
    else
        partition = &noncollideable_object_cluster_partition;
    iterator->cluster_partition = partition;

    return cluster_partition_get_first_cluster(
                    partition, &iterator->reference_index, object->object.first_cluster_reference_index);
}
