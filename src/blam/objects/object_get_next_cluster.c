/* object_get_next_cluster @0x836EBE38 — advance a cluster iterator seeded by object_get_first_cluster; returns
 * the next cluster index (or -1 at the end) via the partition's reference walk. */

#include <stdint.h>
#include "headers/object_cluster_iterator.h"

typedef struct cluster_partition cluster_partition;
extern int16_t cluster_partition_get_next_cluster(const cluster_partition *partition, int *reference_index);

int16_t object_get_next_cluster(object_cluster_iterator *iterator, int object_index)
{
    return cluster_partition_get_next_cluster(iterator->cluster_partition, &iterator->reference_index);
}
