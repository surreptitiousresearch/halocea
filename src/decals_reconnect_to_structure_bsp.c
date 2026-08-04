/* decals_reconnect_to_structure_bsp @0x83741640 — after a structure BSP swap, walk every decal
 * currently on the "disconnected" list and recompute its cluster location from its cached position. If
 * it now resolves to a valid cluster, unlink it from the disconnected list and relink it at the head of
 * that cluster's per-layer decal list (decal_globals->first_decal_indices[layer][cluster]), preserving
 * its layer. Decals that still don't resolve to a cluster remain on the disconnected list untouched.
 * Decal datum stride is 56 bytes (14 dwords), chained through +52 (next) / +48 (previous), matching the
 * sibling decals_delete_permanent_from_cluster.c's already-established field layout. */

#include "headers/data_array.h"
#include "headers/decal_datum.h"
#include "headers/decal_globals.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


extern void scenario_location_from_point(location *location, const real_point3d *point);

void decals_reconnect_to_structure_bsp(void)
{
    if ( !global_decal_data->valid )
        return;

    int decal_index = decal_globals->first_disconnected_decal_index;

    decal_datum *decals = (decal_datum *)global_decal_data->data;
    while ( decal_index != -1 )
    {
        decal_datum *decal = &decals[(unsigned __int16)decal_index];
        int next_disconnected_index = decal->next_decal_index;

        location decal_location;
        scenario_location_from_point(&decal_location, &decal->position);

        if ( (unsigned __int16)decal_location.cluster_index != 0xFFFF )
        {
            int next_index = decal->next_decal_index;
            int previous_index = decal->prev_decal_index;

            if ( next_index != -1 )
                decals[(unsigned __int16)next_index].prev_decal_index = previous_index;

            if ( previous_index == -1 )
                decal_globals->first_disconnected_decal_index = next_index;
            else
                decals[(unsigned __int16)previous_index].next_decal_index = next_index;

            __int16 layer = decal->layer;
            __int16 cluster_index = decal_location.cluster_index;
            int *list_head = &decal_globals->first_decal_indices[layer][cluster_index];
            int old_head = *list_head;

            decal->prev_decal_index = -1;
            decal->cluster_index = cluster_index;
            decal->layer = layer;
            decal->next_decal_index = old_head;

            if ( old_head != -1 )
                decals[(unsigned __int16)old_head].prev_decal_index = decal_index;

            *list_head = decal_index;
        }

        decal_index = next_disconnected_index;
    }
}
