#include "headers/detail_object_global_runtime_data.h"

void structure_detail_objects_flush(void)
{
    detail_object_global_runtime_data->local_player_data[0].last_cell_coord.initialized = 0;
}
