/* cluster_partition_copy @0x837DFD88 — deep-copy a cluster partition: the per-cluster first-reference array (one
 * int per structure-BSP cluster) plus both reference pools. */

#include <string.h>
#include "headers/cluster_partition.h"
#include "headers/structure_bsp.h"

extern void reference_list_copy(data_array *result, const data_array *source);

void cluster_partition_copy(cluster_partition *destination, const cluster_partition *source)
{
    memcpy(destination->cluster_first_data_references,
           source->cluster_first_data_references,
           4 * global_structure_bsp->clusters.count);
    reference_list_copy(destination->cluster_reference_data, source->cluster_reference_data);
    reference_list_copy(destination->data_reference_data, source->data_reference_data);
}
