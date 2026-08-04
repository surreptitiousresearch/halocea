/* path_state_approach_point @0x837D2C60 — find the search node whose surface matches end_surface_index
 * (via linear-probe lookup in the path_state's hash_table, starting at a bucket derived from
 * end_surface_index), then walk that node's parent chain testing each hop's line-of-sight to end_point
 * (structure_test_line2d) until either a hop fails the line test or the chain's root is reached. Reports
 * whether the whole remaining path back to the root is a clear straight line (*straight_line_reference),
 * and the point to approach next: state->input.start_point if the chain reached its root, otherwise the
 * last node's entry_point. Returns 1 if end_surface_index was found in the node set, 0 otherwise.
 *
 * DEVIATION: the decompiler computes node addresses via "(char*)state + 52*node_index" (missing the
 * +132 byte base offset of path_state.node_list) for some reads and the corrected "+132" form for others —
 * both ultimately read the same path_node fields, confirmed by reconciling the word/dword index math
 * against path_node.h's DB-resolved layout (parent_node_index @+2, surface_index @+8, entry_point @+12).
 * Reproduced here using state->node_list[] directly throughout. The hash_table probe is likewise reproduced
 * from a decompiler idiom that computes the array address via state->input.pathfinding_radius's address
 * (offset 0 in path_input) plus a folded constant that lands exactly on state->hash_table — resolved to
 * plain state->hash_table[] indexing. */

#include <stdint.h>
#include "headers/path_state.h"
#include "headers/path_collision_result.h"
#include "headers/real_point2d.h"

#include "headers/structure_bsp.h"
extern uint8_t structure_test_line2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces, const real_point2d *p0, int p0_surface_index, const real_point2d *p1, int p1_surface_index, path_collision_result *result);

extern int path_node_from_hash_table(path_state *state, int surface_index);
uint8_t path_state_approach_point(path_state *state, const real_point2d *end_point, int end_surface_index, uint8_t *straight_line_reference, real_point3d *approach_point_reference)
{
    /* DEVIATION: collapsed compiler-inlined path_node_from_hash_table@0x837D2A98 (zero-xref donor) back into a call; end_surface_index passed straight through, no args folded. This also fixes a pre-existing transcription bug where the increment had been ordered before the fetch (disasm shows fetch-then-increment, matching the donor). */
    __int16 node_index = (__int16)path_node_from_hash_table(state, end_surface_index);

    if ( node_index == -1 )
        return 0;

    const path_node *node = &state->node_list[node_index];

    if ( node->parent_node_index != (__int16)0xFFFF )
    {
        do
        {
            const path_node *parent = &state->node_list[node->parent_node_index];
            path_collision_result collision_result;

            if ( structure_test_line2d(state->structure, state->input.ignore_broken_surfaces, end_point,
                    end_surface_index, (const real_point2d *)&parent->entry_point, parent->surface_index,
                    &collision_result) )
                break;

            node = parent;
        }
        while ( node->parent_node_index != (__int16)0xFFFF );
    }

    if ( node->parent_node_index == (__int16)0xFFFF )
    {
        *straight_line_reference = 1;
        *approach_point_reference = state->input.start_point;
    }
    else
    {
        *straight_line_reference = 0;
        *approach_point_reference = node->entry_point;
    }

    return 1;
}
