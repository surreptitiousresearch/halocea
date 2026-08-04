/* path_node_from_hash_table @0x837D2A98 — open-addressing lookup: probes state->hash_table starting at
 * slot (8*surface_index) mod 4096, linearly scanning by 1 (with wraparound over the 4096-slot table)
 * until an empty slot (-1) or a node whose surface_index matches is found.
 *
 * DEVIATION: the decompiler renders the hash_table access as raw byte-offset pointer math off
 * `&state->input.pathfinding_radius` (that field happens to sit at path_state offset 0, and the byte
 * offset it adds lands exactly on hash_table); rewritten here using the modeled hash_table/nodes fields. */

#include "headers/path_state.h"

int path_node_from_hash_table(path_state *state, int surface_index)
{
    __int16 slot = (8 * surface_index) & 0xFF8;
    __int16 node_index;

    do
    {
        node_index = state->hash_table[slot];
        slot = (slot + 1) & 0xFFF;
    }
    while (node_index != -1 && state->node_list[node_index].surface_index != surface_index);

    return node_index;
}
