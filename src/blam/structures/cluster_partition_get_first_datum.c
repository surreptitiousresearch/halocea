/* cluster_partition_get_first_datum @ 0x837DFFE0 — begin iterating the data referenced by a cluster
 * in a cluster partition. Returns the first datum value and stores the next reference index into
 * `reference_index` (-1 when the cluster has no data).
 *
 * cluster_index is used raw: extsh r11,r5 @0x837DFFE0 and slwi r9,r11,2 @0x837DFFE8 feed
 * lwzx r11,r9,r10 @0x837DFFEC with no bound compare and no sentinel test. The cmpwi cr6,r11,-1
 * @0x837DFFF0 tests the reference that lwzx just loaded into r11, not the index — it is the
 * `reference == -1` test below, not a rejection of an unclustered cluster_index. As-built: keeping -1
 * out of this function is the callers' job (structure_cluster_mark returns 0 for it), not this
 * boundary's. Do not add an index guard here. */

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
