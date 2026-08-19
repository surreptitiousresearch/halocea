/* light_disconnect_from_map @ 0x836F8128 — detach a still-allocated light from the structure BSP's light
 * cluster partition (e.g. when its owner object moves or is hidden). Only acts on lights flagged as
 * map-relevant (flags bit1, the short at light+2); clears the "connected" bit (bit2) afterward. Light
 * datum stride is 124 bytes; the partition link is at light+16.
 *
 * DEVIATION: light_index is the full 32-bit datum handle, not a uint16_t. The clrlwi at 0x836F813C feeds
 * only the datum subscript; 0x836F8140 `mr r4, r3` forwards r3 UNMASKED as cluster_partition_disconnect's
 * datum_index, which that function compares whole (`cmpw r10, r27` @0x837DFF98) against the handle
 * cluster_partition_reconnect stored whole (`stw r25, 4(r11)` @0x837DFED8). Truncating the salt here would
 * stop matching what reconnect stored. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"

struct cluster_partition;

#include "headers/cluster_partition.h"
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);

void light_disconnect_from_map(int light_index)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    if ( (light->flags & (1u << _point_light_connects_to_map_bit)) != 0 )
    {
        cluster_partition_disconnect(&light_cluster_partition, light_index, &light->cluster_reference);
        light->flags &= ~(1u << _point_light_connected_to_map_bit);
    }
}
