/* cluster_get_first_light @0x836F8498 */
#include <stdint.h>
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);

int cluster_get_first_light(int *state, int16_t cluster_index)
{
    return cluster_partition_get_first_datum(&light_cluster_partition, state, cluster_index);
}
