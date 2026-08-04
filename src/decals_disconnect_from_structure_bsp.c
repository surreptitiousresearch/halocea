/* decals_disconnect_from_structure_bsp @0x83741790 — detach every decal from its structure-bsp cluster
 * (called on structure-bsp teardown, before decal_globals' cluster head table is reset): for each of the
 * 5 layers and 512 clusters, walk the cluster's decal chain clearing each decal's cluster_index (making
 * it "unclustered"), and once the tail is reached (next == -1), splice the whole chain onto the front of
 * the disconnected-decal list in one step — the tail's next becomes the old disconnected-list head, that
 * old head's prev becomes the tail, and the disconnected-list head becomes this chain's original head.
 * Decal record layout (56-byte stride, prev@0x30, next@0x34, cluster_index@4) per decal_reinsert.c. */

#include "headers/data_array.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/blam_data_globals.h"


void decals_disconnect_from_structure_bsp(void)
{
    if ( !global_decal_data->valid )
        return;

    for ( int cluster = 0; cluster < 512; cluster = (__int16)(cluster + 1) )
    {
        for ( int layer = 0; layer < 5; layer = (__int16)(layer + 1) )
        {
            int chain_head = decal_globals->first_decal_indices[layer][cluster];
            int decal_index = chain_head;
            if ( decal_index == -1 )
                continue;

            int next_index;
            do
            {
                decal_datum *decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_index);
                next_index = decal->next_decal_index;
                decal->cluster_index = -1;   /* unclustered */

                if ( next_index == -1 )
                {
                    decal->next_decal_index = decal_globals->first_disconnected_decal_index;
                    if ( decal_globals->first_disconnected_decal_index != -1 )
                    {
                        decal_datum *old_disconnected_head = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_globals->first_disconnected_decal_index);
                        old_disconnected_head->prev_decal_index = decal_index;
                    }
                    decal_globals->first_disconnected_decal_index = chain_head;
                    decal_globals->first_decal_indices[layer][cluster] = -1;
                }
                decal_index = next_index;
            }
            while ( next_index != -1 );
        }
    }
}
