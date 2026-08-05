/* collision_surface_find_closest_point2d @0x837E0600 — find the closest point on a collision surface's 2D
 * projection to a query point. The surface's boundary edge loop is projected onto the plane selected by
 * (projection_axis, projection_sign) and walked in order. For each directed edge the query point is tested
 * against the edge's outward side:
 *   - inside every edge  -> the point itself is the answer (returns 1)
 *   - outside an edge, projection landing within the segment -> closest point lies on that edge (returns 0)
 *   - outside, projection past a vertex shared by two such edges -> that vertex is the answer (returns 0)
 *
 * The per-edge "before"/"after" flags (projection before the start vertex / past the end vertex) are
 * carried between iterations so a corner shared by two edges can be detected, including the wrap-around
 * between the last and first edges.
 *
 * DEVIATION: the decompiler rendered the two vertex selections via (4*v20)&0x3FC and (cntlzw(v20)>>3)&4
 * bit tricks; these just pick the edge's start/end vertices according to whether this surface is the
 * edge's right surface (which orients each edge consistently around the loop). Restored to explicit
 * conditional vertex selection. */

#include <stdint.h>

#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"

extern const int16_t global_projection3d_mappings[][2];

int collision_surface_find_closest_point2d(const collision_bsp *bsp, int surface_index,
                                           int16_t projection_axis, uint8_t projection_sign,
                                           const real_point2d *point, real_point2d *result)
{
    int projection = 2 * projection_axis + projection_sign;
    int u_axis = global_projection3d_mappings[projection][0];
    int v_axis = global_projection3d_mappings[projection][1];

    collision_edge *edges = (collision_edge *)bsp->edges.address;
    collision_vertex *vertices = (collision_vertex *)bsp->vertices.address;
    collision_surface *surfaces = (collision_surface *)bsp->surfaces.address;

    int first_edge = surfaces[surface_index].first_edge_index;

    char previous_before = 0, previous_after = 0; /* flags from the prior edge */
    char first_before = 0, first_after = 0;        /* flags from the loop's first edge */
    char before = 0, after = 0;

    int edge_index = first_edge;
    do
    {
        before = 0;
        after = 0;

        collision_edge *edge = &edges[edge_index];
        int surface_on_right = (surface_index == edge->surface_indices[1]);

        /* Orient the edge consistently CCW around this surface. */
        collision_vertex *origin = surface_on_right ? &vertices[edge->vertex_indices[1]] : &vertices[edge->vertex_indices[0]];
        collision_vertex *target = surface_on_right ? &vertices[edge->vertex_indices[0]] : &vertices[edge->vertex_indices[1]];

        float origin_u = origin->point.n[u_axis];
        float origin_v = origin->point.n[v_axis];
        float direction_u = target->point.n[u_axis] - origin_u;
        float direction_v = target->point.n[v_axis] - origin_v;
        float to_point_u = point->n[0] - origin_u;
        float to_point_v = point->n[1] - origin_v;

        float cross = direction_v * to_point_u - direction_u * to_point_v;
        if (cross > 0.0f) /* query point is outside this edge */
        {
            float dot = direction_u * to_point_u + direction_v * to_point_v;
            if (dot >= 0.0f)
            {
                float length_squared = direction_v * direction_v + direction_u * direction_u;
                if (dot <= length_squared)
                {
                    float t = dot / length_squared;
                    result->n[0] = direction_u * t + origin_u;
                    result->n[1] = direction_v * t + origin_v;
                    return 0;
                }
                after = 1; /* projection lies past the end vertex */
            }
            else
            {
                before = 1; /* projection lies before the start vertex */
            }
        }

        if (edge_index == first_edge)
        {
            first_before = before;
            first_after = after;
        }
        else if ((previous_after && (before || !after)) || (before && (previous_after || !previous_before)))
        {
            /* corner between two outward edges -> the shared origin vertex is closest */
            result->n[0] = origin_u;
            result->n[1] = origin_v;
            return 0;
        }

        previous_before = before;
        previous_after = after;
        edge_index = surface_on_right ? edge->edge_indices[1] : edge->edge_indices[0];
    }
    while (edge_index != first_edge);

    if ((after && (first_before || !first_after)) || (first_before && (after || !before)))
    {
        /* wrap-around corner between the last and first edges */
        collision_edge *edge = &edges[edge_index];
        int surface_on_right = (surface_index == edge->surface_indices[1]);
        collision_vertex *origin = surface_on_right ? &vertices[edge->vertex_indices[1]] : &vertices[edge->vertex_indices[0]];
        result->n[0] = origin->point.n[u_axis];
        result->n[1] = origin->point.n[v_axis];
        return 0;
    }

    *result = *point; /* query point is inside the surface */
    return 1;
}
