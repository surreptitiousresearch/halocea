/* light_disconnect_from_map @ 0x836F8128 — detach a still-allocated light from the structure BSP's light
 * cluster partition (e.g. when its owner object moves or is hidden). Only acts on lights flagged as
 * map-relevant (flags bit1, the short at light+2); clears the "connected" bit (bit2) afterward. Light
 * datum stride is 124 bytes; the partition link is at light+16. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"

struct cluster_partition;

#include "headers/cluster_partition.h"
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);

void light_disconnect_from_map(uint16_t light_index)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    if ( (light->flags & (1u << _point_light_connects_to_map_bit)) != 0 )
    {
        cluster_partition_disconnect(&light_cluster_partition, light_index, &light->cluster_reference);
        light->flags &= ~(1u << _point_light_connected_to_map_bit);
    }
}
