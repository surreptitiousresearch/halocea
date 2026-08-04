/* lights_initialize @ 0x836F79E0 — light pool, light globals, cluster partition */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/cluster_partition.h"
#include "headers/lights_game_globals_t.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void *game_state_malloc(const char *name, const char *type, int size);

void lights_initialize(void)
{
    light_data = game_state_data_new("lights", 896, 124);
    lights_game_globals = game_state_malloc("lights globals", 0, 4);
    ((lights_game_globals_t *)lights_game_globals)->render_lights = 1;
    if ( light_data )
        cluster_partition_new(&light_cluster_partition, "light");
}
