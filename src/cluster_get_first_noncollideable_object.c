/* cluster_get_first_noncollideable_object @0x836EBDE8 — begin iterating the noncollideable objects that
 * belong to a structure cluster, via the noncollideable-object cluster partition. */

#include <stdint.h>
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);

int cluster_get_first_noncollideable_object(int *reference_index, int16_t cluster_index)
{
    return cluster_partition_get_first_datum(&noncollideable_object_cluster_partition, reference_index, cluster_index);
}
