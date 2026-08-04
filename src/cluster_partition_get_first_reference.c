/* cluster_partition_get_first_reference @0x837DFDF0 — address of the first-data-reference slot for a cluster
 * (one int per cluster in cluster_first_data_references). */

#include <stdint.h>
#include "headers/cluster_partition.h"

int * cluster_partition_get_first_reference(const cluster_partition *partition, int16_t cluster_index)
{
    return &partition->cluster_first_data_references[cluster_index];
}
