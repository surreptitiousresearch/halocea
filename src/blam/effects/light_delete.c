/* light_delete @ 0x836F7B08 — fully remove a dynamic light: unlink it from the light cluster partition
 * (its connection link lives at light_data int[31*index + 4], i.e. byte +16 of the 124-byte light datum),
 * free the light datum, and tear down the HCEX-side GPU light backing it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/blam_data_globals.h"

struct cluster_partition;

#include "headers/cluster_partition.h"
extern void cluster_partition_disconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference);
extern void datum_delete(data_array *data, int index);
extern void hcex_destroy_light(int light_index);

void light_delete(int light_index)
{
    /* recovered: (int *)light_data->data + 31*idx + 4 -> &light_datum.cluster_reference
     * (31*4 = 124 = sizeof(light_datum), +4*4 = +0x10). */
    cluster_partition_disconnect(&light_cluster_partition, light_index,
        &DATA_ARRAY_ELEMENT(light_data, light_datum, light_index)->cluster_reference);
    datum_delete(light_data, light_index);
    hcex_destroy_light(light_index);
}
