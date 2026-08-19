/* collision_surface_test_sphere @0x837E0FD8 — test one collision surface against the query sphere and, on overlap,
 * record the surface plus its overlapping vertices and edges into the result. Three phases: (1) collect surface
 * vertices inside the sphere, (2) collect surface edges the sphere intersects, (3) if neither hit, test whether the
 * sphere centre projects inside the surface polygon. Destroyed breakable surfaces are skipped. Faithful
 * transcription. */

#include <stdint.h>
#include "headers/test_sphere_data.h"
#include "headers/collision_surface.h"
#include "headers/collision_surface_flags.h"
#include "headers/collision_edge.h"
#include "headers/collision_vertex.h"
#include "headers/real_vector3d.h" /* real_vector3d used by fast_vector_intersects_sphere + edge_vector */
#include "headers/blam_data_globals.h"

extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern void add_feature(int *count, int *indices, int index);

void collision_surface_test_sphere(test_sphere_data *data, int surface_index)
{
    collision_surface *surface = &((collision_surface *)data->bsp->surfaces.address)[surface_index];

    /* skip surfaces that are breakable and currently destroyed */
    unsigned int breakable_index = surface->breakable_surface_index;
    if ( (surface->flags & (1u << _collision_surface_breakable_bit)) != 0
      && breakable_index < data->breakable_surface_count
      /* breakable_surface_flags is a packed bit array read 32 bits at a time (dword-aligned) */
      && (*(const unsigned int *)&data->breakable_surface_flags[(breakable_index >> 3) & 0x1FFFFFFC]
              & (1u << (breakable_index & 0x1F))) == 0 )
    {
        return;
    }

    int first_edge = surface->first_edge_index;
    char hit_any = 0;
    float radius_squared = (data->radius * data->radius);

    /* Phase 1 — vertices inside the sphere */
    int edge_index = first_edge;
    do
    {
        const real_point3d *center = data->center;
        collision_edge *edge = &((collision_edge *)data->bsp->edges.address)[edge_index];
        BOOL forward = (surface_index == edge->surface_indices[1]);
        int vertex_index = edge->vertex_indices[forward];
        const real_point3d *vertex = &((const collision_vertex *)data->bsp->vertices.address)[vertex_index].point;

        float dy = vertex->n[1] - center->n[1];
        float dz = vertex->n[2] - center->n[2];
        if ( (((dz * dz) + (dy * dy))
                   + ((vertex->n[0] - center->n[0]) * (vertex->n[0] - center->n[0]))) <= radius_squared )
        {
            add_feature(&data->result->vertex_count, data->result->vertex_indices, vertex_index);
            hit_any = 1;
        }
        edge_index = edge->edge_indices[forward];
    }
    while ( edge_index != first_edge );

    /* Phase 2 — edges intersecting the sphere */
    edge_index = first_edge;
    do
    {
        float radius = data->radius;
        const real_point3d *center = data->center;
        const collision_vertex *vertices = (const collision_vertex *)data->bsp->vertices.address;
        collision_edge *edge = &((collision_edge *)data->bsp->edges.address)[edge_index];
        BOOL forward = (surface_index == edge->surface_indices[1]);
        int far_vertex = edge->vertex_indices[!forward];
        const real_point3d *near_vertex = &vertices[edge->vertex_indices[forward]].point;

        real_vector3d edge_vector;
        edge_vector.n[0] = vertices[far_vertex].point.n[0] - near_vertex->n[0];
        edge_vector.n[1] = vertices[far_vertex].point.n[1] - near_vertex->n[1];
        edge_vector.n[2] = vertices[far_vertex].point.n[2] - near_vertex->n[2];
        if ( fast_vector_intersects_sphere(near_vertex, &edge_vector, center, radius) )
        {
            add_feature(&data->result->edge_count, data->result->edge_indices, edge_index);
            hit_any = 1;
        }
        edge_index = edge->edge_indices[forward];
    }
    while ( edge_index != first_edge );

    char inside = 1;
    if ( !hit_any )
    {
        /* Phase 3 — sphere centre inside the projected surface polygon */
        const collision_vertex *vertices = (const collision_vertex *)data->bsp->vertices.address;
        int walk_edge = first_edge;
        while ( 1 )
        {
            collision_edge *edge = &((collision_edge *)data->bsp->edges.address)[walk_edge];
            BOOL forward = (surface_index == edge->surface_indices[1]);
            int far_vertex = edge->vertex_indices[!forward];
            const real_point3d *near_vertex = &vertices[edge->vertex_indices[forward]].point;
            /* projection3d mapping entries are position-component indices 0..2 (see
             * src/data/global_projection3d_mappings.c) — in range for real_point3d.n[3] */
            int16_t ia = global_projection3d_mappings[data->projection_axis][data->projection_sign][0];
            int16_t ib = global_projection3d_mappings[data->projection_axis][data->projection_sign][1];

            float cross = (((vertices[far_vertex].point.n[ib] - data->center2d.n[1])
                                        * (near_vertex->n[ia] - data->center2d.n[0]))
                                - ((near_vertex->n[ib] - data->center2d.n[1])
                                        * (vertices[far_vertex].point.n[ia] - data->center2d.n[0])));
            if ( cross < 0.0 )
                break;
            walk_edge = edge->edge_indices[forward];
            if ( walk_edge == first_edge )
                goto resolve;        /* completed the ring → inside */
        }
        inside = 0;
    }

resolve:
    if ( inside )
        add_feature(&data->result->surface_count, data->result->surface_indices, surface_index);
}
