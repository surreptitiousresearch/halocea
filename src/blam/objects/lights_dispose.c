/* lights_dispose @0x836F7A68 — one-time teardown of the lights subsystem: release the light cluster
 * partition. */

#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"

extern void cluster_partition_delete(cluster_partition *partition);

void lights_dispose(void)
{
    cluster_partition_delete(&light_cluster_partition);
}
