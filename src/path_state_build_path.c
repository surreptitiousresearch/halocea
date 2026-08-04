/* path_state_build_path @0x837D3000 — resolves a path_state's search into a path_result: finds the
 * search node whose surface matches the destination (via the same 8*surface_index&0xFF8 hash-with-
 * linear-probe scheme path_state_begin uses to insert nodes), falling back to the closest node reached
 * so far when no exact surface match exists; walks that node's parent chain (up to a 64-node cap) to
 * collect raw waypoints, smooths them (path_smooth), then runs obstacle avoidance (path_avoid_obstacles)
 * to produce the final up-to-4-step path.
 *
 * Clean decompile, no corruption warning; confirmed field/offset math via disasm_range(0x837D3000,
 * 0x837D3270). DEVIATION note: when the "closest node so far" is itself already past the target radius
 * (closest_distance >= destination.target_radius), the compiled code leaves the search-start node index
 * at -1 and loads an as-yet-uninitialized stack slot into a register as if it might become a path_node*
 * — but the very next check is `if (search_node_index == -1) return`, which fires first and returns
 * before that register is ever dereferenced. The garbage load is real but harmless dead code; reproduced
 * by simply not modeling that branch's (unused) node pointer at all, which is behaviorally identical. */

#include <stdint.h>
#include <math.h>
#include <string.h>

#include "headers/path_state.h"
#include "headers/path_result.h"

extern void path_smooth(path_state *state, int16_t raw_step_count, const path_step *raw_steps, int16_t *smoothed_step_count, path_step *smoothed_steps, uint8_t *steps_finish_path);
extern uint8_t path_avoid_obstacles(path_state *state, int16_t input_step_count, const path_step *input_steps, int16_t *avoided_step_count, path_step *avoided_steps, uint8_t *steps_finish_path);

extern int path_node_from_hash_table(path_state *state, int surface_index);
uint8_t path_state_build_path(path_state *state, path_result *path)
{
    path->valid = 0;
    if (!state->destination_valid)
        return path->valid;

    int surface_index = state->destination.surface_index;

    /* DEVIATION: collapsed compiler-inlined path_node_from_hash_table@0x837D2A98 (zero-xref donor) back into a call; surface_index passed straight through, no args folded. */
    __int16 found_node_index = (__int16)path_node_from_hash_table(state, surface_index);

    __int16 search_node_index = found_node_index;

    if (found_node_index == -1)
    {
        /* no exact surface match: fall back to the closest node reached so far, unless that node is
         * already past the destination's target radius (in which case search_node_index stays -1 and
         * we return immediately below — see DEVIATION note above). */
        if (state->closest_distance < state->destination.target_radius)
        {
            search_node_index = state->closest_node_index;
            path->endpoint.point = state->closest_point;
            path->endpoint.surface_index = state->node_list[search_node_index].surface_index;
            path->endpoint.target_radius = state->closest_distance;
        }
    }
    else
    {
        path->endpoint = state->destination;
        path->endpoint.target_radius = 0.0f;
    }

    if (search_node_index == -1)
        return path->valid;

    __int16 depth = state->node_list[search_node_index].depth;

    __int16 raw_step_count = depth + 1;
    if (raw_step_count > 64)
        raw_step_count = 64;

    unsigned __int8 steps_finish_path = 1;
    __int16 smoothed_step_count = 0;
    __int16 avoided_step_count = 0;
    path_step raw_steps[67]; /* stack-confirmed size (67*16 == remaining frame past this buffer's start) */
    path_step smoothed_steps[4];
    path_step avoided_steps[4];

    path_node *previous_node = NULL;
    __int16 node_index = search_node_index;
    do
    {
        path_node *node = &state->node_list[node_index];
        __int16 node_depth = node->depth;

        if (node_depth < 64)
        {
            raw_steps[node_depth].surface_index = node->surface_index;
            if (!previous_node)
                raw_steps[node_depth].point = path->endpoint.point;
            else
                raw_steps[node_depth].point = previous_node->entry_point;
        }
        else
        {
            steps_finish_path = 0;
        }

        previous_node = node;
        node_index = node->parent_node_index;
    } while (node_index != -1);

    path_smooth(state, raw_step_count, raw_steps, &smoothed_step_count, smoothed_steps, &steps_finish_path);
    if (!path_avoid_obstacles(state, smoothed_step_count, smoothed_steps, &avoided_step_count, avoided_steps,
            &steps_finish_path))
        return path->valid;

    path->valid = 1;
    path->step_index = 0;
    path->step_count = (char)avoided_step_count;
    path->steps_finish_path = steps_finish_path;
    memcpy(path->steps, avoided_steps, sizeof(path_step) * avoided_step_count);

    unsigned __int8 result = path->valid;
    if (path->steps_finish_path)
    {
        path_step *last_step = &path->steps[path->step_count - 1];
        path->endpoint.point = last_step->point;
        path->endpoint.surface_index = last_step->surface_index;

        float dx = state->destination.point.n[0] - path->endpoint.point.n[0];
        float dy = state->destination.point.n[1] - path->endpoint.point.n[1];
        float dz = state->destination.point.n[2] - path->endpoint.point.n[2];
        path->endpoint.target_radius = sqrtf(dx * dx + dy * dy + dz * dz);
    }
    return result;
}
