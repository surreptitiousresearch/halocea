/* collision_surface_test_pill @0x837E1D20 — sweep the test pill against every edge of one collision surface.
 * Walks the surface's edge ring; for each edge it builds the edge segment (two vertices) and calls
 * pill_test_vector. On a nearer hit it records the fraction, builds the contact-plane normal as (ray-hit point −
 * closest edge point), normalises it, and stores the surface/material into data->result. Returns TRUE if any edge
 * produced a nearer contact. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/collision_surface.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_vector3d.h"
#include "headers/test_pill_data.h"

extern float __fsqrts(float);
extern double __fabs(double x);
extern uint8_t pill_test_vector(const real_point3d *base, const real_vector3d *edge, float width, const real_point3d *vertices, const real_point3d *origin, const real_vector3d *direction, float *results);

uint8_t collision_surface_test_pill(test_pill_data *data, int surface_index)
{
    collision_surface *surface = &((collision_surface *)data->bsp->surfaces.address)[surface_index];
    int struck = 0;
    int first_edge = surface->first_edge_index;
    int edge_index = first_edge;

    do
    {
        const real_vector3d *direction = data->vector;
        const real_point3d  *origin    = data->point;
        float                radius    = data->radius;
        const collision_vertex *vertices = (const collision_vertex *)data->bsp->vertices.address;
        collision_edge *edge = &((collision_edge *)data->bsp->edges.address)[edge_index];

        /* Pick the edge orientation so the segment runs in this surface's winding order. */
        int forward = (surface_index == edge->surface_indices[1]);
        /* DEVIATION: decompiler branchless selects (4*forward)&0x3FC and (_cntlzw(forward)>>3)&4
         * reduce to vertex_indices[forward] / vertex_indices[1-forward] for forward in {0,1}. */
        int near_vertex = edge->vertex_indices[forward];
        int far_vertex  = edge->vertex_indices[1 - forward];
        const real_point3d *vertex_a = &vertices[near_vertex].point;
        const float        *vertex_b = vertices[far_vertex].point.n;

        real_vector3d edge_vector;
        edge_vector.n[0] = vertex_b[0] - vertex_a->n[0];
        edge_vector.n[1] = vertex_b[1] - vertex_a->n[1];
        edge_vector.n[2] = vertex_b[2] - vertex_a->n[2];

        float results[2];
        if ( pill_test_vector(vertex_a, &edge_vector, radius, &vertices->point, origin, direction, results)
          && data->result->t > results[0] )
        {
            float t = results[0];
            float u = results[1];
            data->result->t = t;

            const float *dir   = data->vector->n;
            const float *start = data->point->n;
            float edge_pt_x = ((edge_vector.n[0] * u) + vertex_a->n[0]);
            float edge_pt_y = ((edge_vector.n[1] * u) + vertex_a->n[1]);
            float ray_x = ((dir[0] * t) + start[0]);
            float ray_y = ((dir[1] * t) + start[1]);
            float normal_z = (((dir[2] * t) + start[2])
                                   - ((edge_vector.n[2] * u) + vertex_a->n[2]));

            data->result->plane.normal.n[2] = normal_z;
            data->result->plane.normal.n[1] = (ray_y - edge_pt_y);
            data->result->plane.normal.n[0] = (ray_x - edge_pt_x);

            float nx = data->result->plane.normal.n[0];
            float ny = data->result->plane.normal.n[1];
            float nz = data->result->plane.normal.n[2];
            float length = __fsqrts(((nz * nz)
                                                 + ((nx * nx) + (ny * ny))));
            if ( __fabs(length) >= 0.000099999997 )
            {
                float inverse = ((float)1.0 / length);
                data->result->plane.normal.n[0] = nx * inverse;
                data->result->plane.normal.n[1] = ny * inverse;
                data->result->plane.normal.n[2] = nz * inverse;
            }

            struck = 1;
            data->result->plane.distance = 3.4028235e38f;
            data->result->surface_index = surface_index;
            data->result->material_index = surface->material_index;
        }

        /* Advance to the next edge around this surface. */
        edge_index = edge->edge_indices[forward];
    }
    while ( edge_index != first_edge );

    return struck;
}
