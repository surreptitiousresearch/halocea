/* decal_reinsert @0x837413A8 — insert a decal at the head of its (layer, cluster) linked list. Decal record
 * layout (56-byte stride, prev@0x30, next@0x34, cluster_index@4, layer_index@6) established by
 * decal_delete.c / decals_delete_permanent_from_cluster.c. */

#include "headers/data_array.h"
#include "headers/decal_datum.h"
#include "headers/decal_globals.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>

/* decal_index stays int: stored full-word (stw @0x83741410/0x83741414), clrlwi is the
 * DATA_ARRAY_ELEMENT handle mask */
void decal_reinsert(int decal_index, int16_t cluster_index, int16_t layer)
{
    decal_datum *decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_index);
    int old_head = decal_globals->first_decal_indices[layer][cluster_index];

    decal->prev_decal_index = -1;
    decal->cluster_index = cluster_index;
    decal->layer = layer;
    decal->next_decal_index = old_head;

    if ( old_head != -1 )
    {
        decal_datum *old_head_decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, old_head);
        old_head_decal->prev_decal_index = decal_index;
    }

    decal_globals->first_decal_indices[layer][cluster_index] = decal_index;
}
