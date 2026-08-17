/* get_local_player_datum @0x837115A8 */
#include <stdint.h>
#include "headers/detail_object_global_runtime_data.h"

struct detail_object_global_runtime_data * get_local_player_datum(int16_t local_player_index)
{
    /* recovered: (char *)base + 21008 * local_player_index -> &...->local_player_data[local_player_index] (21008 == sizeof(detail_object_runtime_data)) */
    return (struct detail_object_global_runtime_data *)
        &detail_object_global_runtime_data->local_player_data[local_player_index];
}
