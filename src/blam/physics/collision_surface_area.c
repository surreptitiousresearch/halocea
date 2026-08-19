/* collision_surface_area @0x837E01E8 — compute the surface area of one collision-BSP surface by walking its
 * boundary edge loop, fanning triangles from the loop's anchor vertex, and summing the signed areas of those
 * triangles projected onto the surface plane's (facing-corrected) normal. Returns max(0, accumulated area);
 * the value is twice the true polygon area (no 0.5 factor is applied). The edge walk chooses each vertex and
 * the next edge from the winding side (this surface as the edge's left vs right surface).
 *
 * DEVIATION: register-pun doubles are plain float math; the returned `*((float*)&v5+1)` is the float held in
 * fp1 = max(0, area).
 *
 * DEVIATION: plane_designator bit 31 is the facing flag, not index data. The binary never masks before
 * indexing because the 16-byte stride shift discards that bit in 32-bit arithmetic (`slwi r10, r3, 4`
 * @0x837E0234), and isolates it separately for the facing test (`clrrwi r10, r3, 31` @0x837E0244). The
 * `& 0x7FFFFFFF` below is therefore a proven no-op on the as-built target and keeps the index in range
 * where ptrdiff_t is wider (x64); the facing test still reads the unmasked value. */

#include "headers/collision_bsp.h"
#include "headers/real_plane3d.h"
#include "headers/real_vector3d.h"

float collision_surface_area(const collision_bsp *bsp, int surface_index)
{
    const collision_edge   *edges    = (const collision_edge *)bsp->edges.address;
    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const collision_surface *surface = &((const collision_surface *)bsp->surfaces.address)[surface_index];

    const float *plane = (const float *)&((const real_plane3d *)bsp->bsp3d.planes.address)[surface->plane_designator & 0x7FFFFFFF];
    real_vector3d normal;
    if ( surface->plane_designator >= 0 )
    {
        normal.n[0] = plane[0];
        normal.n[1] = plane[1];
        normal.n[2] = plane[2];
    }
    else
    {
        normal.n[0] = -plane[0];
        normal.n[1] = -plane[1];
        normal.n[2] = -plane[2];
    }

    int first_edge_index = surface->first_edge_index;
    const collision_edge *edge = &edges[first_edge_index];

    /* Anchor the fan at the first edge's trailing vertex (chosen by which side this surface is on). */
    int anchor_on_right = surface_index == edge->surface_indices[1];
    const collision_vertex *anchor = &vertices[anchor_on_right ? edge->vertex_indices[1] : edge->vertex_indices[0]];

    const collision_edge *next_edge = &edges[anchor_on_right ? edge->edge_indices[1] : edge->edge_indices[0]];
    int on_right = surface_index == next_edge->surface_indices[1];
    int next_edge_index = on_right ? next_edge->edge_indices[1] : next_edge->edge_indices[0];

    float area = 0.0f;
    if ( next_edge_index != first_edge_index )
    {
        float ax = anchor->point.n[0];
        float ay = anchor->point.n[1];
        float az = anchor->point.n[2];
        do
        {
            const collision_vertex *leading  = &vertices[on_right ? next_edge->vertex_indices[1]   : next_edge->vertex_indices[0]];
            const collision_vertex *trailing = &vertices[on_right ? next_edge->vertex_indices[0] : next_edge->vertex_indices[1]];
            next_edge = &edges[next_edge_index];

            float e1x = leading->point.n[0] - ax;
            float e1y = leading->point.n[1] - ay;
            float e1z = leading->point.n[2] - az;
            float e2x = trailing->point.n[0] - ax;
            float e2y = trailing->point.n[1] - ay;
            float e2z = trailing->point.n[2] - az;

            on_right = surface_index == next_edge->surface_indices[1];

            area += (e1z * e2x - e2z * e1x) * normal.n[1]
                  + ((e2z * e1y - e2y * e1z) * normal.n[0]
                   + (e2y * e1x - e1y * e2x) * normal.n[2]);

            next_edge_index = on_right ? next_edge->edge_indices[1] : next_edge->edge_indices[0];
        }
        while ( next_edge_index != first_edge_index );
    }

    return area > 0.0f ? area : 0.0f;
}
