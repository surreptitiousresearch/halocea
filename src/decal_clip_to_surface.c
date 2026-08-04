/* decal_clip_to_surface @0x83740CA8 — clip one decal projection onto one collision-BSP surface,
 * accumulating the clipped polygon into a decal_geometry scratch buffer. Two modes, chosen per surface
 * by the angle between the surface plane and the projection plane:
 *   - wrap-queue mode (update_queue set AND angle > minimum_wrap_angle): the surface deviates too much
 *     to clip flat, so instead its edge loop is walked and every neighbour surface whose shared edge
 *     intersects the exclusion sphere is appended (deduplicated) to surface_queue; if the angle is still
 *     within minimum_skip_angle the surface itself is recorded in deviant_surface_list for the wrapped
 *     re-projection pass in decal_new_from_collision.
 *   - flat-clip mode (otherwise): Sutherland-Hodgman clips the projected decal quad against every edge
 *     of the surface (ping-pong through decal_points2d_temp), then emits the surviving polygon into the
 *     geometry buffer with texture coordinates derived from the projection's u/v axes; original
 *     (unclipped) vertices are nudged 1/256 world units off the surface along its plane normal. While
 *     flat-clipping with update_queue set, any edge the clip actually split also queues its neighbour
 *     surface (the decal hangs over that edge).
 *
 * DEVIATION (FPR-shadow ABI): float `radius` in f1 burns the r7 shadow slot, so the decompiler misnames
 * every trailing parameter (its `surface_queue` r8 is really `type`, its `surface_queue_write_index` r9
 * is really `surface_queue`, its `deviant_surface_list` r10 is really `surface_queue_write_index`, and
 * the real deviant_surface_list/deviant_surface_count are the stack args it calls a29/a31). Mapping
 * confirmed from the prologue (0x83740CC0-0x83740CE0) and matches the DB 10-arg prototype used verbatim.
 * DEVIATION: the decompiler's `v78` register pun (surface->first_edge_index reinterpreted as a float in a
 * real_point2d) is two independent lives of r10 — disasm 0x83741170-0x83741190 is a plain 8-byte
 * previous_point = current_point copy (ld/std) followed by the next-edge load; the loop terminator is
 * simply next_edge != surface->first_edge_index. */

#include <stdint.h>
#include "headers/decal_projection.h"
#include "headers/collision_surface_flags.h"
#include "headers/decal_geometry.h"
#include "headers/decal_wrap_parameters.h"
#include "headers/collision_bsp.h"
#include "headers/real_plane2d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/real_vector3d.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern const __int16 global_projection3d_mappings[1][6][2];

extern real_plane3d *bsp3d_get_plane_from_designator(const bsp3d *bsp, int plane_designator, real_plane3d *result);
extern float angle_between_normals3d(const real_vector3d *a, const real_vector3d *b);
extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern real_plane2d *plane2d_from_points(real_plane2d *plane, const real_point2d *point0, const real_point2d *point1);
extern int16_t convex_polygon2d_clip_to_plane(int16_t count, const real_point2d *points, const real_plane2d *plane, int16_t maximum_count, real_point2d *result, uint32_t *clip_flags, uint8_t *clipped, float epsilon);
extern real_point3d *project_point2d(const real_point2d *p2d, const real_plane3d *plane, int16_t projection, uint8_t sign, real_point3d *p3d);

void decal_clip_to_surface(const decal_projection *projection, decal_geometry *geometry, int surface_index,
                           uint8_t update_queue, float radius, int16_t type, int *surface_queue,
                           int16_t *surface_queue_write_index, int *deviant_surface_list,
                           int16_t *deviant_surface_count)
{

    if (surface_index == -1)
        return;

    collision_bsp *bsp = global_collision_bsp;
    collision_surface *surface = &((collision_surface *)bsp->surfaces.address)[surface_index];

    __int16 queue_count;   /* var_10E — only live when update_queue is set (all reads are gated) */
    __int16 deviant_count; /* var_10C */
    if (update_queue)
    {
        queue_count = *surface_queue_write_index;
        deviant_count = *deviant_surface_count;
    }

    real_plane3d surface_plane;
    bsp3d_get_plane_from_designator(&bsp->bsp3d, surface->plane_designator, &surface_plane);
    float wrap_angle = angle_between_normals3d(&surface_plane.n, &projection->plane.n);

    if (update_queue && wrap_angle > decal_wrap_parameters[type].minimum_wrap_angle * DEG_TO_RAD)
    {
        /* wrap-queue mode: too steep to clip flat — queue every neighbour whose shared edge
         * intersects the exclusion sphere around the projection origin */
        int edge_index = surface->first_edge_index;
        do
        {
            collision_vertex *vertices = (collision_vertex *)bsp->vertices.address;
            collision_edge *edge = &((collision_edge *)bsp->edges.address)[edge_index];
            int is_right = surface_index == edge->surface_indices[1];
            int vertex_a = is_right ? edge->vertex_indices[0] : edge->vertex_indices[1];

            if (queue_count < 1024)
            {
                float exclusion_radius = decal_wrap_parameters[type].radius_exclusion_multiplier * radius;
                int vertex_b = is_right ? edge->vertex_indices[1] : edge->vertex_indices[0];
                real_vector3d edge_vector;
                edge_vector.n[0] = vertices[vertex_b].point.n[0] - vertices[vertex_a].point.n[0];
                edge_vector.n[1] = vertices[vertex_b].point.n[1] - vertices[vertex_a].point.n[1];
                edge_vector.n[2] = vertices[vertex_b].point.n[2] - vertices[vertex_a].point.n[2];
                if (fast_vector_intersects_sphere(&vertices[vertex_a].point, &edge_vector,
                                                  &projection->basis.position, exclusion_radius))
                {
                    int neighbor_surface = is_right ? edge->surface_indices[0] : edge->surface_indices[1];
                    if (neighbor_surface != -1)
                    {
                        for (__int16 i = 0; i < queue_count; i++)
                        {
                            if (surface_queue[i] == neighbor_surface)
                                goto next_edge; /* already queued */
                        }
                        surface_queue[queue_count++] = neighbor_surface;
                    }
                }
            }
next_edge:
            edge_index = is_right ? edge->edge_indices[1] : edge->edge_indices[0];
        } while (edge_index != surface->first_edge_index);

        if (wrap_angle <= decal_wrap_parameters[type].minimum_skip_angle * DEG_TO_RAD)
        {
            if (deviant_count < 1024)
                deviant_surface_list[deviant_count++] = surface_index;
        }
    }
    else
    {
        /* flat-clip mode: clip the projected decal quad against every edge of the surface */
        int edge_index = surface->first_edge_index;
        __int16 edge_counter = 0;
        unsigned int clip_flags = 0; /* per-output-vertex "emitted by a clip" bits */
        const real_point2d *input_points = projection->decal_points2d;
        real_point2d *output_points = 0; /* always set — the edge loop runs at least once */
        __int16 vertex_count = 4;
        real_point2d previous_point;
        real_point2d current_point;

        do
        {
            collision_vertex *vertices = (collision_vertex *)bsp->vertices.address;
            collision_edge *edge = &((collision_edge *)bsp->edges.address)[edge_index];
            output_points = decal_points2d_temp[edge_counter & 1]; /* ping-pong rows of 12 */
            int is_right = surface_index == edge->surface_indices[1];
            int loop_vertex = is_right ? edge->vertex_indices[0] : edge->vertex_indices[1];
            int map = 2 * projection->axis + projection->sign;

            if (!edge_counter)
            {
                /* first iteration: seed the previous point from the edge's other endpoint */
                int other_vertex = is_right ? edge->vertex_indices[1] : edge->vertex_indices[0];
                previous_point.n[0] = vertices[other_vertex].point.n[global_projection3d_mappings[0][map][0]];
                previous_point.n[1] = vertices[other_vertex].point.n[global_projection3d_mappings[0][map][1]];
            }

            current_point.n[0] = vertices[loop_vertex].point.n[global_projection3d_mappings[0][map][0]];
            current_point.n[1] = vertices[loop_vertex].point.n[global_projection3d_mappings[0][map][1]];

            real_plane2d clip_plane;
            if (plane2d_from_points(&clip_plane, &current_point, &previous_point))
            {
                unsigned __int8 clipped;
                vertex_count = convex_polygon2d_clip_to_plane(vertex_count, input_points, &clip_plane, 12,
                                                              output_points, &clip_flags, &clipped, 0.0f);
                if (update_queue && clipped && queue_count < 1024)
                {
                    /* the clip split this edge — the decal hangs over it: queue the neighbour */
                    int vertex_b = is_right ? edge->vertex_indices[1] : edge->vertex_indices[0];
                    float exclusion_radius = decal_wrap_parameters[type].radius_exclusion_multiplier * radius;
                    real_vector3d edge_vector;
                    edge_vector.n[0] = vertices[vertex_b].point.n[0] - vertices[loop_vertex].point.n[0];
                    edge_vector.n[1] = vertices[vertex_b].point.n[1] - vertices[loop_vertex].point.n[1];
                    edge_vector.n[2] = vertices[vertex_b].point.n[2] - vertices[loop_vertex].point.n[2];
                    if (fast_vector_intersects_sphere(&vertices[loop_vertex].point, &edge_vector,
                                                      &projection->basis.position, exclusion_radius))
                    {
                        int neighbor_surface = is_right ? edge->surface_indices[0] : edge->surface_indices[1];
                        if (neighbor_surface != -1)
                        {
                            for (__int16 i = 0; i < queue_count; i++)
                            {
                                if (surface_queue[i] == neighbor_surface)
                                    goto advance_edge; /* already queued */
                            }
                            surface_queue[queue_count++] = neighbor_surface;
                        }
                    }
                }
            }
            else
            {
                vertex_count = 0;
            }
advance_edge:
            previous_point = current_point;
            input_points = output_points;
            edge_counter++;
            edge_index = is_right ? edge->edge_indices[1] : edge->edge_indices[0];
        } while (edge_index != surface->first_edge_index && vertex_count > 0);

        /* emit the surviving polygon (skip degenerate results, full buffers, and flagged surfaces) */
        if (vertex_count >= 3
            && vertex_count <= 1024 - geometry->decal_vertex_count
            && (surface->flags & (1u << _collision_surface_two_sided_bit)) == 0
            && (surface->flags & (1u << _collision_surface_invisible_bit)) == 0
            && (surface->flags & (1u << _collision_surface_breakable_bit)) == 0)
        {
            geometry->decal_surface_vertex_counts[geometry->decal_surface_count++] = vertex_count;
            for (__int16 i = 0; i < vertex_count; i++)
            {
                const real_point2d *point = &output_points[i];
                float du = point->n[0] - projection->decal_points2d[0].n[0];
                float dv = point->n[1] - projection->decal_points2d[0].n[1];
                decal_vertex_internal *vertex = &geometry->decal_vertices[geometry->decal_vertex_count];

                vertex->texcoord.n[0] = (projection->texture_v_axis.n[1] * du
                                       - dv * projection->texture_v_axis.n[0]) * projection->texture_scale;
                vertex->texcoord.n[1] = -((projection->texture_u_axis.n[1] * du
                                         - dv * projection->texture_u_axis.n[0]) * projection->texture_scale);
                project_point2d(point, &surface_plane, projection->axis, projection->sign, &vertex->position);

                if (((1 << i) & clip_flags) == 0)
                {
                    /* original (unclipped) vertex: lift it 1/256 off the surface along the normal */
                    vertex->position.n[0] += surface_plane.n.n[0] * 0.00390625f;
                    vertex->position.n[1] += surface_plane.n.n[1] * 0.00390625f;
                    vertex->position.n[2] += surface_plane.n.n[2] * 0.00390625f;
                }
                geometry->decal_vertex_count++;
            }
        }
    }

    if (update_queue)
    {
        *surface_queue_write_index = queue_count;
        *deviant_surface_count = deviant_count;
    }
}
