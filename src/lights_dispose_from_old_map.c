/* lights_dispose_from_old_map @0x836F7AD4 — per-map light teardown: invalidate the light datum
 * array and its cluster partition. */

#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/blam_data_globals.h"
#include "headers/cluster_partition.h"
extern void cluster_partition_make_invalid(cluster_partition *partition);

extern void data_make_invalid(data_array *data);

void lights_dispose_from_old_map(void)
{
    data_make_invalid(light_data);
    cluster_partition_make_invalid(&light_cluster_partition);
}
