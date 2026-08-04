/* hs_node_gc @0x8372ED50 — garbage-collect the HaloScript syntax-node array: delete every node whose
 * "permanent" flag (bit 3 of the node's flags word) is clear, leaving only nodes that belong to compiled
 * scenario scripts.
 *
 * The decompiler's raw datum-index arithmetic for the flags word reduces to HS_SYNTAX_NODE(i).flags (offset
 * 0x06 of the 20-byte node). The return is ABI residue (data_next_index == -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_syntax_node.h"
#include "headers/hs_syntax_node_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void datum_delete(data_array *data, int index);

void hs_node_gc(void)
{
    for ( int i = data_next_index(hs_syntax_data, -1); i != -1; i = data_next_index(hs_syntax_data, i) )
    {
        if ( (HS_SYNTAX_NODE(i).flags & (1u << _hs_syntax_node_permanent_bit)) == 0 )
            datum_delete(hs_syntax_data, i);
    }
}
