/* cluster_partition_delete @0x837DFC90 — clear the partition's three pointers. The pools themselves are owned by
 * the game-state arena; this only drops the references. */

#include "headers/cluster_partition.h"

void cluster_partition_delete(cluster_partition *partition)
{
    if ( partition->cluster_first_data_references )
        partition->cluster_first_data_references = 0;
    if ( partition->cluster_reference_data )
        partition->cluster_reference_data = 0;
    if ( partition->data_reference_data )
        partition->data_reference_data = 0;
}
