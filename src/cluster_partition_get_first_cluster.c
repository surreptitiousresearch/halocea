/* cluster_partition_get_first_cluster @0x837DFD08 — begin iterating the cluster references chained from a given
 * head reference. Seeds *reference_index, then returns the first cluster index (reference +4) and advances the
 * cursor to the next link. Returns -1 if the chain is empty. */

#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/data_reference.h"
#include <stdint.h>

int16_t cluster_partition_get_first_cluster(const cluster_partition *partition, int *reference_index, int first_cluster_reference)
{
    *reference_index = first_cluster_reference;
    if ( first_cluster_reference == -1 )
        return -1;

    const data_reference *reference =
        &((const data_reference *)partition->cluster_reference_data->data)[(unsigned short)first_cluster_reference];
    *reference_index = reference->next_reference_index;
    return reference->datum_index;
}
