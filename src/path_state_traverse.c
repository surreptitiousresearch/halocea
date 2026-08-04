/* path_state_traverse @0x837D34B8 — one full A* search over a path_state's working set. Repeatedly pops
 * the lowest-cost node from the min-heap, and (unless it is the destination surface, or already worse than
 * the best-so-far bound) expands it: for every boundary edge of its surface it computes the edge-crossing
 * entry point (pulled toward the destination when the edge is long enough), the linear/attractor/heuristic
 * costs, then finds-or-creates the neighbour node through the surface-index hash table and relaxes it into
 * the heap. Impassable edges (not marked passable, leading back to the parent, or crossing an unbroken
 * breakable surface) are skipped. Throughout it tracks the closest approach to the destination. Returns
 * whether a path was found: true when there is no destination, or the closest approach is within the
 * destination's target radius.
 *
 * The decompiler based the popped-node accesses at `(char*)state + 52*node_index` (= &state->node_list[i] minus
 * 0x84); every such access is reproduced here as a named path_node field off &state->node_list[node_index]. */

#include <stdint.h>
#include "headers/path_state.h"
#include "headers/path_edge.h"
#include "headers/real_point2d.h"
#include "headers/collision_bsp.h"
#include "headers/PATHFINDING_SURFACE_flags.h"
#include "headers/collision_surface_flags.h"
#include "headers/collision_surface.h"
#include "headers/path_constants.h"

#include "headers/structure_bsp.h"
extern int16_t build_path_edges_for_surface(const structure_bsp *structure, int surface_index, path_edge *edges);
extern void path_heap_bubble_down(path_state *state, int16_t heap_location);
extern void path_heap_bubble_up(path_state *state, int16_t heap_index);
extern float path_attractor_weight(const path_state *state, const real_point3d *p0, const real_point3d *p1, float *distance_reference);
extern const uint8_t *breakable_surface_flags_get(void);
extern int collision_surface_find_closest_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point, real_point2d *result);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);
extern float __fsqrts(float x);

uint8_t path_state_traverse(path_state *state)
{
    float pathfinding_radius = (state->input.pathfinding_radius >= 0.2f) ? state->input.pathfinding_radius : 0.2f;
    path_edge edges[68];
    int16_t node_index;

    while ( 1 )
    {
        /* pop the lowest-cost node off the binary min-heap (heap[1] is the root; slot 0 unused) */
        node_index = -1;
        if ( state->heap_count > 1 )
        {
            node_index = state->heap[1].node_index;
            state->node_list[node_index].heap_location = -1;
            int16_t new_count = (int16_t)(state->heap_count - 1);
            state->heap_count = new_count;
            if ( new_count > 1 )
            {
                state->heap[1].node_index = state->heap[new_count].node_index;
                state->heap[1].quantized_cost_estimate = state->heap[new_count].quantized_cost_estimate;
                path_heap_bubble_down(state, 1);
            }
        }
        if ( node_index == -1 )
            return !state->destination_valid || state->closest_distance <= (double)state->destination.target_radius;

        path_node *node = &state->node_list[node_index];
        if ( state->destination_valid )
        {
            if ( node->surface_index == state->destination.surface_index )
                break; /* reached the destination surface */

            /* prune: node already costs more than the best-so-far bound */
            double bound = state->closest_distance;
            if ( bound < 5.0 )
                bound = 5.0;
            if ( node->total_cost_estimate > (double)((float)(bound * 10.0) + state->closest_cost_estimate) )
                return !state->destination_valid || state->closest_distance <= (double)state->destination.target_radius;
        }

        int edge_count = build_path_edges_for_surface(state->structure, node->surface_index, edges);
        for ( int i = 0; i < edge_count; i++ )
        {
            path_edge *edge = &edges[i];
            int adjacent_surface = edge->adjacent_surface_index;

            /* passable if the edge is flagged passable and does not lead straight back to the parent */
            int passable = (edge->adjacent_pathfinding_surface & (1u << PATHFINDING_SURFACE_WALKABLE_BIT)) != 0
                         && node->parent_node_surface_index != edge->adjacent_surface_index;

            if ( !state->input.ignore_broken_surfaces
              && (edge->adjacent_pathfinding_surface & (1u << PATHFINDING_SURFACE_BREAKABLE_BIT)) != 0 )
            {
                /* breakable surface: a BROKEN surface is a hole — not walkable. This block is the
                 * inlined body of surface_is_broken() (bit CLEAR in the breakable_surface_flags
                 * vector = broken; cf. collision_surface_test_sphere's "destroyed" skip). */
                collision_bsp *bsp = (collision_bsp *)state->structure->collision_bsp.address;
                collision_surface *surface = &((collision_surface *)bsp->surfaces.address)[adjacent_surface];
                int is_broken = 0;
                if ( (surface->flags & (1u << _collision_surface_breakable_bit)) != 0 )
                {
                    uint8_t breakable_index = surface->breakable_surface_index;
                    /* dword-aligned packed-bit read, same idiom as surface_is_broken.c */
                    is_broken = ((1 << (breakable_index & 0x1F))
                                 & *(int *)&breakable_surface_flags_get()[(breakable_index >> 3) & 0x1FFFFFFC]) == 0;
                }
                if ( is_broken )
                    passable = 0;
            }
            if ( !passable )
                continue;

            /* entry point = edge midpoint, pulled toward the destination along the edge when it is long */
            real_point3d entry_point;
            entry_point.n[0] = edge->edge_vector.n[0] * 0.5f + edge->base_point.n[0];
            entry_point.n[1] = edge->edge_vector.n[1] * 0.5f + edge->base_point.n[1];
            entry_point.n[2] = edge->edge_vector.n[2] * 0.5f + edge->base_point.n[2];

            if ( state->destination_valid )
            {
                float ex = edge->edge_vector.n[0];
                float ey = edge->edge_vector.n[1];
                float ez = edge->edge_vector.n[2];
                float edge_len_sq = ex * ex + (ez * ez + ey * ey);
                if ( edge_len_sq > 16.0f
                  && edge_len_sq > ((pathfinding_radius * 2.0f) * (pathfinding_radius * 2.0f)) )
                {
                    float margin = pathfinding_radius / __fsqrts(edge_len_sq);
                    float t = ((ex * (state->destination.point.n[0] - edge->base_point.n[0]))
                               + (ez * (state->destination.point.n[2] - edge->base_point.n[2]))
                               + ((state->destination.point.n[1] - edge->base_point.n[1]) * ey))
                              / edge_len_sq;
                    if ( t < margin )
                        t = margin;
                    else if ( t > 1.0f - margin )
                        t = 1.0f - margin;
                    entry_point.n[0] = ex * t + edge->base_point.n[0];
                    entry_point.n[1] = ey * t + edge->base_point.n[1];
                    entry_point.n[2] = ez * t + edge->base_point.n[2];
                }
            }

            /* linear step distance from the parent node's entry point */
            float dx = entry_point.n[0] - node->entry_point.n[0];
            float dy = entry_point.n[1] - node->entry_point.n[1];
            float dz = entry_point.n[2] - node->entry_point.n[2];
            float edge_len = __fsqrts(dx * dx + (dy * dy + dz * dz));
            float path_dist = node->path_distance_from_origin + edge_len;

            float attractor_closest;
            float cost_increment;
            if ( state->input.attractor_valid )
            {
                float attractor_dist;
                float weight = path_attractor_weight(state, &node->entry_point, &entry_point, &attractor_dist);
                cost_increment = (weight + 1.0f) * edge_len;
                if ( node->closest_approach_to_attractor <= (double)attractor_dist )
                    attractor_closest = node->closest_approach_to_attractor;
                else
                    attractor_closest = attractor_dist;
            }
            else
            {
                attractor_closest = 0.0f;
                cost_increment = edge_len;
            }

            float cumulative = node->cumulative_cost + cost_increment;
            float total_estimate = cumulative;
            float goal_dist = 0.0f;
            if ( state->destination_valid )
            {
                float gx = state->destination.point.n[0] - entry_point.n[0];
                float gy = state->destination.point.n[1] - entry_point.n[1];
                float gz = state->destination.point.n[2] - entry_point.n[2];
                goal_dist = __fsqrts(gx * gx + (gz * gz + gy * gy));
                total_estimate = goal_dist + cumulative;
            }
            int quantized = (int)(total_estimate * 10.0f);
            int16_t quantized16 = (int16_t)quantized;

            /* find-or-create the neighbour node keyed by its surface index in the open hash table */
            int target_index = -1;
            if ( quantized < 0x7FFF
              && (!state->input.search_bounded || path_dist <= (double)state->input.search_maximum_distance) )
            {
                /* open-address hash: 8-aligned start slot, linear probe modulo PATH_HASH_TABLE_SIZE */
                int slot = (8 * adjacent_surface) & (PATH_HASH_TABLE_SIZE - 8);
                int16_t slot_value = state->hash_table[slot];
                while ( slot_value != -1 && state->node_list[slot_value].surface_index != adjacent_surface )
                {
                    slot = (int16_t)(slot + 1) & (PATH_HASH_TABLE_SIZE - 1);
                    slot_value = state->hash_table[slot];
                }
                if ( slot_value == -1 )
                {
                    /* empty slot: allocate a fresh node */
                    int16_t nc = state->node_count;
                    if ( nc < PATH_NODE_LIST_SIZE )
                    {
                        state->node_count = nc + 1;
                        state->hash_table[slot] = nc;
                        target_index = nc;
                        state->node_list[nc].heap_location = -1;
                    }
                }
                else if ( quantized < state->node_list[slot_value].quantized_cost_estimate
                       && (uint16_t)state->node_list[slot_value].heap_location != 0xFFFF )
                {
                    /* relax an existing open node only if this path is cheaper */
                    target_index = slot_value;
                }
            }
            if ( target_index == -1 )
                continue;

            path_node *target = &state->node_list[target_index];
            int16_t old_heap_location = target->heap_location;
            target->parent_node_index = node_index;
            target->linear_distance_to_entry_point = edge_len;
            target->closest_approach_to_attractor = attractor_closest;
            target->entry_point.n[0] = entry_point.n[0];
            target->path_distance_from_origin = path_dist;
            target->entry_point.n[1] = entry_point.n[1];
            target->cumulative_cost = cumulative;
            target->total_cost_estimate = total_estimate;
            target->surface_index = adjacent_surface;
            target->entry_point.n[2] = entry_point.n[2];
            target->parent_node_surface_index = node->surface_index;
            target->quantized_cost_estimate = quantized16;
            target->depth = node->depth + 1;

            if ( old_heap_location == -1 )
            {
                /* not in the heap yet: push it */
                int16_t hc = state->heap_count;
                if ( hc < PATH_NODE_LIST_SIZE )
                {
                    state->heap_count = hc + 1;
                    state->heap[hc].node_index = target_index;
                    state->heap[hc].quantized_cost_estimate = quantized16;
                    path_heap_bubble_up(state, hc);
                }
            }
            else
            {
                /* already open: update its cost and re-sort */
                state->heap[old_heap_location].quantized_cost_estimate = quantized16;
                path_heap_bubble_up(state, old_heap_location);
            }

            if ( state->destination_valid )
            {
                real_point3d closest_candidate;
                closest_candidate.n[0] = target->entry_point.n[0];
                closest_candidate.n[1] = target->entry_point.n[1];
                closest_candidate.n[2] = target->entry_point.n[2];
                if ( goal_dist < 4.0f )
                {
                    /* near the goal: project the destination onto the surface for the exact closest point */
                    const collision_bsp *bsp = (const collision_bsp *)state->structure->collision_bsp.address;
                    real_point2d projected;
                    /* xy-prefix pun: destination's leading real_point3d point read as a real_point2d */
                    collision_surface_find_closest_point2d(bsp, adjacent_surface, 2, 1,
                            (const real_point2d *)&state->destination, &projected);
                    collision_surface_project_point2d(bsp, adjacent_surface, 2, 1, &projected, &closest_candidate);
                    float cx = closest_candidate.n[0] - state->destination.point.n[0];
                    float cy = closest_candidate.n[1] - state->destination.point.n[1];
                    float cz = closest_candidate.n[2] - state->destination.point.n[2];
                    goal_dist = __fsqrts(cx * cx + (cz * cz + cy * cy));
                }
                if ( goal_dist < state->closest_distance )
                {
                    state->closest_distance = goal_dist;
                    state->closest_cost_estimate = total_estimate;
                    state->closest_node_index = target_index;
                    state->closest_point.n[0] = closest_candidate.n[0];
                    state->closest_point.n[1] = closest_candidate.n[1];
                    state->closest_point.n[2] = closest_candidate.n[2];
                }
            }
        }
    }

    /* reached the destination surface exactly: the closest point is the destination itself */
    state->closest_distance = 0.0f;
    state->closest_node_index = node_index;
    state->closest_point.n[0] = state->destination.point.n[0];
    state->closest_point.n[1] = state->destination.point.n[1];
    state->closest_point.n[2] = state->destination.point.n[2];
    return !state->destination_valid || state->closest_distance <= (double)state->destination.target_radius;
}
