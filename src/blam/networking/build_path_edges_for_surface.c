/* build_path_edges_for_surface @0x837D2988 — walk a collision surface's boundary edge loop (up to 64
 * edges, or fewer if the loop closes sooner) and emit a path_edge per boundary edge: the adjacent surface
 * across that edge (and whether it's itself a pathfinding surface), plus the edge's base point and
 * direction vector. Follows collision_edge's forward_edge/reverse_edge links depending on whether this
 * surface is the edge's left or right surface (per their established meaning in collision_bsp.h).
 * Returns the number of edges emitted. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/path_edge.h"

int16_t build_path_edges_for_surface(const structure_bsp *structure, int surface_index, path_edge *edges)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;
    const unsigned char *pathfinding_flags = (const unsigned char *)structure->pathfinding_surfaces.address;

    int16_t edge_count = 0;
    const collision_surface *surface = &((const collision_surface *)bsp->surfaces.address)[surface_index];
    int first_edge_index = surface->first_edge_index;
    int edge_index = first_edge_index;

    do
    {
        const collision_edge *edge = &((const collision_edge *)bsp->edges.address)[edge_index];
        path_edge *output = &edges[edge_count];
        edge_count = (int16_t)(edge_count + 1);

        uint8_t is_right_surface = edge->surface_indices[1] == surface_index;
        int adjacent_surface_index = is_right_surface ? edge->surface_indices[0] : edge->surface_indices[1];

        output->adjacent_surface_index = adjacent_surface_index;
        output->adjacent_pathfinding_surface = pathfinding_flags[adjacent_surface_index];

        const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
        const real_point3d *start_point = &vertices[edge->vertex_indices[0]].point;
        const real_point3d *end_point = &vertices[edge->vertex_indices[1]].point;

        output->base_point.n[0] = start_point->n[0];
        output->base_point.n[1] = start_point->n[1];
        output->base_point.n[2] = start_point->n[2];
        output->edge_vector.n[0] = end_point->n[0] - start_point->n[0];
        output->edge_vector.n[1] = end_point->n[1] - start_point->n[1];
        output->edge_vector.n[2] = end_point->n[2] - start_point->n[2];

        if ( edge_count == 64 )
            break;

        edge_index = is_right_surface ? edge->edge_indices[1] : edge->edge_indices[0];
    }
    while ( edge_index != first_edge_index );

    return edge_count;
}
