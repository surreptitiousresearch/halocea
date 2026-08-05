/* cluster_partition_get_next_datum @ 0x837DFCC8 — advance a cluster-partition iterator: return the
 * datum value at the current reference and store the following reference into `reference_index`
 * (-1 when exhausted). */

#include <stdint.h>
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/data_reference.h"

int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index)
{
    const data_reference *datum;

    if ( *reference_index == -1 )
        return -1;

    datum = &((const data_reference *)partition->data_reference_data->data)[(uint16_t)*reference_index];
    *reference_index = datum->next_reference_index;
    return datum->datum_index;
}
