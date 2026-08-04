/* light_build_cluster_array @0x836F7CD0 — collect the structure clusters a light touches into cluster_indices
 * (up to maximum_count) by walking the light cluster partition from the light's partition reference. Returns
 * the number of clusters written. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"


#include "headers/cluster_partition.h"
extern int16_t cluster_partition_get_first_cluster(const cluster_partition *partition, int *reference_index, int first_cluster_reference);
extern int16_t cluster_partition_get_next_cluster(const cluster_partition *partition, int *reference_index);

int light_build_cluster_array(uint16_t light_index, int16_t maximum_count, int16_t *cluster_indices)
{
    int count = 0;
    int reference_index[12];

    for (__int16 cluster = cluster_partition_get_first_cluster(&light_cluster_partition, reference_index,
                                                               *((int *)light_data->data + 31 * light_index + 4));
         count < maximum_count;
         cluster = cluster_partition_get_next_cluster(&light_cluster_partition, reference_index))
    {
        if (cluster == -1)
            break;
        cluster_indices[(__int16)count] = cluster;
        count = (__int16)(count + 1);
    }

    return count;
}
