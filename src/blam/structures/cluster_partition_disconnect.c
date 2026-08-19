/* cluster_partition_disconnect @ 0x837DFF10 — remove an object (datum_index) from every cluster it is
 * currently linked into. first_cluster_reference heads the object's list of cluster references (chained
 * through reference+8). For each one: read which cluster it points at (reference+4), free the cluster
 * reference, then walk that cluster's data-reference list (head at partition->cluster_first_data_references
 * [cluster], chained through ref+8) to find and unlink the entry pointing back at datum_index (ref+4).
 * Finally resets the object's cluster-reference head to -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_reference.h"
#include "headers/cluster_partition.h"

extern void datum_delete(data_array *data, int index);

void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference)
{
    for ( int cluster_reference_index = *first_cluster_reference; cluster_reference_index != -1; )
    {
        data_array *cluster_reference_data = partition->cluster_reference_data;
        data_reference *cluster_reference =
            DATA_ARRAY_ELEMENT(cluster_reference_data, data_reference, cluster_reference_index);
        int cluster_index = cluster_reference->datum_index;
        int next_cluster_reference_index = cluster_reference->next_reference_index;
        datum_delete(cluster_reference_data, cluster_reference_index);

        data_array *data_reference_data = partition->data_reference_data;
        int *link = &partition->cluster_first_data_references[(int16_t)cluster_index];
        if ( *link != -1 )
        {
            while ( 1 )
            {
                data_reference *data_ref = DATA_ARRAY_ELEMENT(data_reference_data, data_reference, *link);
                if ( data_ref->datum_index == datum_index )
                {
                    datum_delete(data_reference_data, *link);
                    *link = data_ref->next_reference_index;
                    break;
                }
                link = &data_ref->next_reference_index;
                if ( data_ref->next_reference_index == -1 )
                    break;
            }
        }

        cluster_reference_index = next_cluster_reference_index;
    }

    *first_cluster_reference = -1;
}
