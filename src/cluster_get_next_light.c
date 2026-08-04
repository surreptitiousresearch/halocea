#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

#include "headers/cluster_partition.h"
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);

int cluster_get_next_light(int *state)
{
    return cluster_partition_get_next_datum(&light_cluster_partition, state);
}
