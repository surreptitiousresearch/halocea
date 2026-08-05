/* path_state_begin @0x837D32F0 — seeds a path_state's search with its single starting node: rejects if
 * no start point was recorded (`input.start_surface_index == -1`) or the start point's z is below
 * -1000 (an out-of-world sentinel). If a destination was set, computes the straight-line distance from
 * start to destination (also used as this node's `total_cost_estimate` heuristic and to derive its
 * `quantized_cost_estimate`, distance*10 rounded, rejecting if that would overflow an __int16); otherwise
 * both are 0. Appends node 0 (surface/point copied from the start, parent indices -1, costs/heuristic as
 * above), tracks it as the closest-to-destination node so far when a destination is set, hashes it by
 * surface index, and pushes it onto the search heap.
 *
 * Clean decompile, no corruption. disasm_range(0x837D32F0, 0x837D3498) resolved the exact byte layout of
 * `path_state.node_list`/`heap` (see path_state.h) — the decompiler's own `(char*)state + 52*node_count`
 * address computation is genuinely `state->node_list[node_count]`, its base offset (0x84) having been folded
 * into the later `+0x84`-relative field-store displacements instead of showing up as a separate `nodes`
 * field access; likewise the heap slot address folds a large `+13346` (`0x3422`) additive constant that
 * is actually the heap array's own byte offset (53382) divided by 4. Both were recovered by walking the
 * raw `mulli`/`slwi`/`sthx` instructions directly rather than trusting the decompiler's variable shapes. */

#include <stdint.h>
#include <math.h>
#include <float.h>

#include "headers/path_state.h"

extern void path_heap_bubble_up(path_state *state, int16_t heap_index);

uint8_t path_state_begin(path_state *state)
{
    if (state->input.start_surface_index == -1 || state->input.start_point.n[2] <= -1000.0f)
        return 0;

    float distance_to_destination;
    int16_t quantized_cost_estimate;
    if (state->destination_valid)
    {
        float dx = state->destination.point.n[0] - state->input.start_point.n[0];
        float dy = state->destination.point.n[1] - state->input.start_point.n[1];
        float dz = state->destination.point.n[2] - state->input.start_point.n[2];
        distance_to_destination = sqrtf(dx * dx + dy * dy + dz * dz);

        int scaled = (int)(distance_to_destination * 10.0f);
        if (scaled >= 0x7FFF)
            return 0;
        quantized_cost_estimate = (int16_t)scaled;
    }
    else
    {
        distance_to_destination = 0.0f;
        quantized_cost_estimate = 0;
    }

    int16_t node_index = state->node_count;
    state->node_count = node_index + 1;

    path_node *node = &state->node_list[node_index];
    node->parent_node_index = -1;
    node->parent_node_surface_index = -1;
    node->surface_index = state->input.start_surface_index;
    node->entry_point = state->input.start_point;
    node->linear_distance_to_entry_point = 0.0f;
    node->closest_approach_to_attractor = FLT_MAX;
    node->path_distance_from_origin = 0.0f;
    node->cumulative_cost = 0.0f;
    node->total_cost_estimate = distance_to_destination;
    node->quantized_cost_estimate = quantized_cost_estimate;
    node->depth = 0;

    if (state->destination_valid)
    {
        state->closest_distance = distance_to_destination;
        state->closest_cost_estimate = distance_to_destination;
        state->closest_node_index = node_index;
        state->closest_point = state->input.start_point;
    }

    state->hash_table[((16 * node->surface_index) & 0x1FF0) / 2] = node_index;

    if (state->heap_count < 1024)
    {
        int16_t heap_index = state->heap_count;
        state->heap_count = heap_index + 1;
        state->heap[heap_index].node_index = node_index;
        state->heap[heap_index].quantized_cost_estimate = quantized_cost_estimate;
        path_heap_bubble_up(state, heap_index);
    }

    return 1;
}
