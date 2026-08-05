/* decals_delete_permanent_from_cluster @ 0x83741878 — remove the permanent decals belonging to one
 * structure cluster (called when that cluster leaves the PVS). Decals are bucketed into 5 layers; for a
 * real cluster index each layer's per-cluster list head is decal_globals->first_decal_indices[layer][cluster],
 * while a cluster index of -1 sweeps only the "disconnected" list (layer 0). Each decal in the list
 * chained through decal+52 that is flagged permanent (flags short[1] bit1) has the flag cleared, the
 * global permanent counter decremented, and its rasterizer vertices freed. Decal datum stride is 56 bytes.
 *
 * The decompiler renders this with gotos around a constant cluster index; reconstructed as a flat sweep
 * over the 5 layers with the head selected per the cluster == -1 special case. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_decal_vertices_delete(int cache_index);

void decals_delete_permanent_from_cluster(int16_t cluster_index)
{
    if ( !global_decal_data->valid )
        return;

    for ( int layer = 0; layer < 5; ++layer )
    {
        int decal_index;
        if ( cluster_index == -1 )
        {
            if ( layer != 0 )
                continue;                       /* disconnected list lives only in layer 0 */
            decal_index = decal_globals->first_disconnected_decal_index;
        }
        else
        {
            decal_index = decal_globals->first_decal_indices[layer][cluster_index];
        }

        while ( decal_index != -1 )
        {
            decal_datum *decal = DATUM_GET(global_decal_data, decal_datum, decal_index);
            int16_t flags = decal->flags;
            int next_decal_index = decal->next_decal_index;
            if ( (flags & (1u << _decal_permanent_bit)) != 0 )
            {
                decal->flags = flags & ~(1u << _decal_permanent_bit);
                --decal_globals->permanent_count;
                rasterizer_decal_vertices_delete(decal_index);
            }
            decal_index = next_decal_index;
        }
    }
}
