/* cluster_partition_get_next_cluster @0x837DFD48 — advance a cluster-reference cursor: returns the cluster index
 * at the cursor and advances to the next link, or -1 at the end. */

#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/data_reference.h"
#include <stdint.h>

int16_t cluster_partition_get_next_cluster(const cluster_partition *partition, int *reference_index)
{
    if ( *reference_index == -1 )
        return -1;

    const data_reference *reference =
        &((const data_reference *)partition->cluster_reference_data->data)[(unsigned short)*reference_index];
    *reference_index = reference->next_reference_index;
    return reference->datum_index;
}
