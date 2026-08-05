/* cluster_partition_get_first_datum @ 0x837DFFE0 — begin iterating the data referenced by a cluster
 * in a cluster partition. Returns the first datum value and stores the next reference index into
 * `reference_index` (-1 when the cluster has no data). */

#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/data_reference.h"
#include <stdint.h>

int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index,
                                      int16_t cluster_index)
{
    int reference = partition->cluster_first_data_references[cluster_index];
    const data_reference *datum;

    *reference_index = reference;
    if ( reference == -1 )
        return -1;

    datum = &((const data_reference *)partition->data_reference_data->data)[(uint16_t)reference];
    *reference_index = datum->next_reference_index;
    return datum->datum_index;
}
