/* lights_initialize_for_new_map @0x836F7A78 — revalidate the light datum array, enable light rendering, and
 * reset the light cluster partition for a new map. */

#include "headers/cluster_partition.h"
#include "headers/data_array.h"


#include "headers/lights_game_globals_t.h"
#include "headers/blam_data_globals.h"

extern void data_make_valid(data_array *data);
extern void cluster_partition_make_valid(cluster_partition *partition);

void lights_initialize_for_new_map(void)
{
    data_make_valid(light_data);
    ((lights_game_globals_t *)lights_game_globals)->render_lights = 1;
    cluster_partition_make_valid(&light_cluster_partition);
}
