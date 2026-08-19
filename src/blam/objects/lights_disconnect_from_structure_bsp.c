/* lights_disconnect_from_structure_bsp @0x836F8198 — run when the active structure BSP is torn down.
 * Walks every live light; for each light flagged connected-to-map (0x4, _point_light_connected_to_map_bit)
 * that also connects-to-map (0x2, _point_light_connects_to_map_bit), removes it from the light cluster
 * partition and then re-stamps the connected flag so lights_reconnect_to_structure_bsp will re-place it.
 *
 * DEVIATION: the clear-then-set of bit 0x4 around cluster_partition_disconnect is faithful to the shipped
 * code (the flag is briefly cleared, then restored for the reconnect pass). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/cluster_partition.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);

void lights_disconnect_from_structure_bsp(void)
{
    int result = data_next_index(light_data, -1);
    for ( int i = result; result != -1; i = result )
    {
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, i);
        int16_t flags = light->flags;
        if ( (flags & (1u << _point_light_connected_to_map_bit)) != 0 )
        {
            if ( (flags & (1u << _point_light_connects_to_map_bit)) != 0 )
            {
                cluster_partition_disconnect(&light_cluster_partition, i, &light->cluster_reference);
                light->flags &= ~(1u << _point_light_connected_to_map_bit);
            }
            light->flags |= (1u << _point_light_connected_to_map_bit);
        }
        result = data_next_index(light_data, i);
    }
}
