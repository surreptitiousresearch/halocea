/* path_state_estimated_distance @0x837D2DA0 — given a completed/partial path search state and a goal
 * (end_point on end_surface_index), estimate the remaining travel distance, the closest the path comes to the
 * search attractor, and a normalized "which way to head next" direction. It finds the search node sitting on the
 * goal surface via the state's surface->node hash table (linear-probed), then:
 *   - distance = straight-line(end_point, node.entry_point) + node.path_distance_from_origin;
 *   - closest attractor approach = distance from the attractor to the closest point on the final segment,
 *     clamped to the node's own recorded closest approach (0 when no attractor is active);
 *   - estimated direction = (a point ~0.8 world-units along the reconstructed path from the start, or the goal
 *     itself if the path is shorter than that) minus the start point, normalized.
 * To reconstruct the forward direction it first threads each node's child_node_index by walking the parent chain
 * from the goal node up to the root, then walks that child chain back down accumulating segment lengths.
 * Returns 1 when the goal node was found; otherwise fills the outputs with FLT_MAX / the zero vector and returns 0.
 *
 * Node/hash offsets are the typed path_state layout: hash_table[(8*surface)&0xFF8 ..] linear-probe, nodes[i] at
 * 0x84+52*i. (The decompiler renders the node base with the 0x84 folded into a real_point3d* index; recovered to
 * named path_node fields here.) */

#include "headers/path_state.h"
#include "headers/path_node.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern void closest_point_to_attractor(const real_point3d *p0, const real_point3d *p1, const real_point3d *q, real_point3d *result);
extern float normalize3d(real_vector3d *v);

extern int path_node_from_hash_table(path_state *state, int surface_index);
int path_state_estimated_distance(path_state *state, const real_point3d *end_point, int end_surface_index,
        float *distance_reference, float *closest_approach_to_attractor_reference,
        real_vector3d *estimated_direction_reference)
{
    /* DEVIATION: collapsed compiler-inlined path_node_from_hash_table@0x837D2A98 (zero-xref donor) back into a call; end_surface_index passed straight through; the host's local while(1)/goto not_found tail is preserved to feed the function's existing not-found error path. */
    __int16 goal_node_index = (__int16)path_node_from_hash_table(state, end_surface_index);
    if ( goal_node_index == -1 )
        goto not_found;

    path_node *goal_node = &state->node_list[goal_node_index];

    float dx = end_point->n[0] - goal_node->entry_point.n[0];
    float dy = end_point->n[1] - goal_node->entry_point.n[1];
    float dz = end_point->n[2] - goal_node->entry_point.n[2];
    float distance = __fsqrts(dy * dy + (dx * dx + dz * dz)) + goal_node->path_distance_from_origin;

    float closest_approach;
    if ( state->input.attractor_valid )
    {
        real_point3d closest;
        closest_point_to_attractor(&goal_node->entry_point, end_point, &state->input.attractor_point, &closest);
        float ax = closest.n[0] - state->input.attractor_point.n[0];
        float ay = closest.n[1] - state->input.attractor_point.n[1];
        float az = closest.n[2] - state->input.attractor_point.n[2];
        closest_approach = __fsqrts(ay * ay + (az * az + ax * ax));
        if ( closest_approach > goal_node->closest_approach_to_attractor )
            closest_approach = goal_node->closest_approach_to_attractor;
    }
    else
    {
        closest_approach = 0.0f;
    }

    if ( closest_approach_to_attractor_reference )
        *closest_approach_to_attractor_reference = closest_approach;
    *distance_reference = distance;

    if ( estimated_direction_reference )
    {
        /* Thread child links by walking the parent chain from the goal node up to the root. */
        __int16 previous_node_index = -1;
        __int16 current_node_index = goal_node_index;
        __int16 parent_node_index;
        do
        {
            state->node_list[current_node_index].child_node_index = previous_node_index;
            previous_node_index = current_node_index;
            parent_node_index = state->node_list[current_node_index].parent_node_index;
            current_node_index = parent_node_index;
        }
        while ( parent_node_index != -1 );

        /* Walk the child chain back down from the root, accumulating ~0.8 world-units of path length. */
        __int16 walk_node_index = previous_node_index;   /* == root of the path */
        path_node *walk_node = goal_node;
        float accumulated_distance = 0.0f;
        if ( walk_node_index != -1 )
        {
            do
            {
                if ( accumulated_distance >= 0.80000001f )
                    break;
                walk_node = &state->node_list[walk_node_index];
                accumulated_distance += walk_node->linear_distance_to_entry_point;
                walk_node_index = walk_node->child_node_index;
            }
            while ( walk_node_index != -1 );
        }

        const real_point3d *heading_point;
        if ( walk_node_index == -1 )
            heading_point = end_point;
        else
            heading_point = &walk_node->entry_point;

        estimated_direction_reference->n[0] = heading_point->n[0] - state->input.start_point.n[0];
        estimated_direction_reference->n[1] = heading_point->n[1] - state->input.start_point.n[1];
        estimated_direction_reference->n[2] = heading_point->n[2] - state->input.start_point.n[2];
        normalize3d(estimated_direction_reference);
    }

    return 1;

not_found:
    if ( closest_approach_to_attractor_reference )
        *closest_approach_to_attractor_reference = 3.4028235e38f;
    if ( estimated_direction_reference )
        *estimated_direction_reference = *global_zero_vector3d;
    *distance_reference = 3.4028235e38f;
    return 0;
}
