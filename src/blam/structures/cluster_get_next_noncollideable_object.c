/* cluster_get_next_noncollideable_object @0x836EBE00 — continue iterating the noncollideable objects in a
 * structure cluster, via the noncollideable-object cluster partition. */

#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);

int cluster_get_next_noncollideable_object(int *reference_index)
{
    return cluster_partition_get_next_datum(&noncollideable_object_cluster_partition, reference_index);
}
