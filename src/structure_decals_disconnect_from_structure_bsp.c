/* structure_decals_disconnect_from_structure_bsp @0x837E2D70 — when disconnecting from a structure_bsp
 * (e.g. unloading the old map), deletes the permanent decals of every cluster that has any runtime decal
 * records. Cluster stride 104 bytes, matching structure_decals_update.c's has_decals test exactly. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/blam_data_globals.h"

extern void decals_delete_permanent_from_cluster(int16_t cluster_index);

void structure_decals_disconnect_from_structure_bsp(void)
{
    structure_bsp *bsp = global_structure_bsp;
    if ( !bsp->runtime_decals.count )
        return;

    int16_t cluster_count = (int16_t)bsp->clusters.count;
    for ( int16_t cluster_index = 0; cluster_index < cluster_count; ++cluster_index )
    {
        structure_cluster *cluster = (structure_cluster *)bsp->clusters.address + cluster_index;
        unsigned char has_decals =
            ((uint16_t)cluster->first_runtime_decal_index != 0xFFFF && cluster->runtime_decal_count)
                ? 1 : 0;

        if ( has_decals )
            decals_delete_permanent_from_cluster(cluster_index);
    }
}
