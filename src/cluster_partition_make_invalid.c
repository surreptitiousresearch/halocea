/* cluster_partition_make_invalid @0x837DFC40 — invalidate the partition's two reference pools (cluster and data)
 * if currently valid. */

#include "headers/cluster_partition.h"

extern void data_make_invalid(data_array *data);

void cluster_partition_make_invalid(cluster_partition *partition)
{
    data_array *cluster_reference_data = partition->cluster_reference_data;
    if ( cluster_reference_data->valid )
        data_make_invalid(cluster_reference_data);
    data_array *data_reference_data = partition->data_reference_data;
    if ( data_reference_data->valid )
        data_make_invalid(data_reference_data);
}
