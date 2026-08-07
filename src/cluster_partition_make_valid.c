/* cluster_partition_make_valid @0x837DFBF8 — reset a cluster partition: clear the per-cluster first-reference
 * heads (2KB of -1) and revalidate the cluster-reference and data-reference datum arrays. */

#include <string.h>
#include "headers/cluster_partition.h"
#include "headers/data_array.h"

extern void data_make_valid(data_array *data);

void cluster_partition_make_valid(cluster_partition *partition)
{
    memset(partition->cluster_first_data_references, -1, 0x800u);
    data_make_valid(partition->cluster_reference_data);
    data_make_valid(partition->data_reference_data);
}
