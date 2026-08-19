/* structure_detail_objects_initialize_for_new_map @0x837119E0 — reset the rasterizer detail-object state and
 * zero the global runtime data, invalidating the cached cell coordinate. */

#include <string.h>
#include "headers/detail_object_global_runtime_data.h"

extern void rasterizer_detail_objects_initialize_for_new_map(void);

void structure_detail_objects_initialize_for_new_map(void)
{
    struct detail_object_global_runtime_data *runtime = detail_object_global_runtime_data;

    rasterizer_detail_objects_initialize_for_new_map();
    memset(runtime, 0, sizeof(*detail_object_global_runtime_data));
    runtime->local_player_data[0].last_cell_coord.initialized = 0;
}
