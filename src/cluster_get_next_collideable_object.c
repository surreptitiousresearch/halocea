/* cluster_get_next_collideable_object @ 0x836EBE28 — advance the collideable-object iterator started
 * by cluster_get_first_collideable_object; returns the next object index or -1 when exhausted. */

#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);

int cluster_get_next_collideable_object(int *reference_index)
{
    return cluster_partition_get_next_datum(&collideable_object_cluster_partition, reference_index);
}
