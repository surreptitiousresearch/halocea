/* cluster_get_first_collideable_object @ 0x836EBE10 — begin iterating the collideable objects that
 * belong to a structure cluster, via the collideable-object cluster partition. */

#include <stdint.h>
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);

int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index)
{
    return cluster_partition_get_first_datum(&collideable_object_cluster_partition, reference_index, cluster_index);
}
