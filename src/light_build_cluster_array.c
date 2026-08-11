/* light_build_cluster_array @0x836F7CD0 — collect the structure clusters a light touches into cluster_indices
 * (up to maximum_count) by walking the light cluster partition from the light's partition reference. Returns
 * the number of clusters written. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/light_datum.h"
#include "headers/blam_data_globals.h"


#include "headers/cluster_partition.h"
extern int16_t cluster_partition_get_first_cluster(const cluster_partition *partition, int *reference_index, int first_cluster_reference);
extern int16_t cluster_partition_get_next_cluster(const cluster_partition *partition, int *reference_index);

int light_build_cluster_array(int light_index, int16_t maximum_count, int16_t *cluster_indices)
{
    int count = 0;
    int reference_index[12];

    /* recovered: (int *)light_data->data + 31*idx + 4 -> light_datum.cluster_reference
     * (31*4 = 124 = sizeof(light_datum), +4*4 = +0x10). */
    for (int16_t cluster = cluster_partition_get_first_cluster(
             &light_cluster_partition, reference_index,
             DATA_ARRAY_ELEMENT(light_data, light_datum, light_index)->cluster_reference);
         count < maximum_count;
         cluster = cluster_partition_get_next_cluster(&light_cluster_partition, reference_index))
    {
        if (cluster == -1)
            break;
        cluster_indices[(int16_t)count] = cluster;
        count = (int16_t)(count + 1);
    }

    return count;
}
