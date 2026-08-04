/* build_torus @0x837FD168 — procedural torus tessellator. Emits a triangle-strip torus into
 * caller-supplied buffers: (ring_segment_count+1) rings around the major circle, each holding
 * (cylinder_segment_count+1) vertices around the minor (tube) circle. For every vertex it writes a
 * position (points[]), a UV (texture_uvs[]), and — for every ring after the first — the triangle-strip
 * index pairs bridging this ring to the previous one (triangle_strip_vertex_indices[]). The final
 * vertex of each ring/cylinder loop is a seam-closing copy of the loop's first vertex so the strip
 * wraps cleanly. Total emitted vertex and strip counts are returned through the two reference args.
 *
 * Reconstructed from disassembly (0x837FD168-0x837FD4A8): the decompiler emitted
 * "local variable allocation has failed" and rendered the sin/cos calls as long-double scratch
 * aliases of the cylinder_radius stack slot and the trailing cylinder_segment_count stack arg (arg_56)
 * as a phantom parameter a30. The real ABI (from the prologue) is the 10-arg signature below:
 * matrix/refs/buffers in r3..r8, ring_segment_count in r9, ring_radius in f1, cylinder_segment_count
 * on the stack, cylinder_radius in f2. Per-step (float) casts are preserved for bit-exactness. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/math_constants.h"
#include <math.h>
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern float __fabs(float x);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

void build_torus(const real_matrix4x3 *matrix, int16_t *vertex_count_reference, int16_t *triangle_strip_count_reference, real_point3d *points, real_point2d *texture_uvs, int16_t *triangle_strip_vertex_indices, int16_t ring_segment_count, float ring_radius, int16_t cylinder_segment_count, float cylinder_radius)
{
    __int16 vertex_count = 0;
    __int16 triangle_strip_count = 0;

    if ( ring_segment_count >= 0 )
    {
        float ring_segment_count_float = (float)ring_segment_count;

        for ( int ring_index = 0;
              ring_index <= ring_segment_count;
              ring_index = (__int16)(ring_index + 1) )
        {
            float ring_fraction = ((float)ring_index / ring_segment_count_float);
            float ring_angle = (ring_fraction * TWO_PI);

            float ring_sine = (float)sin(ring_angle);
            float ring_cosine = (float)cos(ring_angle);

            float ring_sine_radius = (ring_sine * ring_radius);
            float ring_cosine_radius = (ring_cosine * ring_radius);

            /* axis = global_up3d x (ring_sine_radius, 0, ring_cosine_radius), then normalized */
            real_vector3d axis;
            axis.n[0] = ((global_up3d->n[2] * ring_sine_radius)
                    - (global_up3d->n[1] * 0.0f));
            axis.n[2] = ((global_up3d->n[1] * ring_cosine_radius)
                    - (global_up3d->n[0] * ring_sine_radius));
            axis.n[1] = ((global_up3d->n[0] * 0.0f)
                    - (global_up3d->n[2] * ring_cosine_radius));

            float axis_length = __fsqrts(((axis.n[2] * axis.n[2])
                    + ((axis.n[0] * axis.n[0]) + (axis.n[1] * axis.n[1]))));
            if ( __fabs(axis_length) >= 0.000099999997f )
            {
                float inverse_length = (1.0f / axis_length);
                axis.n[0] = (inverse_length * axis.n[0]);
                axis.n[1] = (inverse_length * axis.n[1]);
                axis.n[2] = (inverse_length * axis.n[2]);
            }

            float cylinder_sine_radius = (ring_sine * cylinder_radius);
            float cylinder_cosine_radius = (ring_cosine * cylinder_radius);

            if ( cylinder_segment_count >= 0 )
            {
                float ring_texture_coordinate = (ring_fraction * 2.0f);

                for ( int cylinder_index = 0;
                      cylinder_index <= cylinder_segment_count;
                      cylinder_index = (__int16)(cylinder_index + 1) )
                {
                    texture_uvs->n[1] = ring_texture_coordinate;

                    if ( ring_index > 0 )
                    {
                        if ( cylinder_index == 0 )
                        {
                            *triangle_strip_vertex_indices = 2 * (cylinder_segment_count + 1);
                            ++triangle_strip_count;
                            ++triangle_strip_vertex_indices;
                        }
                        triangle_strip_vertex_indices[0] = vertex_count;
                        triangle_strip_vertex_indices[1] = vertex_count - cylinder_segment_count - 1;
                        triangle_strip_vertex_indices += 2;
                    }

                    if ( ring_index == ring_segment_count )
                    {
                        /* close the major (ring) seam: copy the matching vertex from ring 0 */
                        int ring_wrap = (cylinder_segment_count + 1) * ring_segment_count;
                        points->n[0] = points[-ring_wrap].n[0];
                        points->n[1] = points[-ring_wrap].n[1];
                        points->n[2] = points[-ring_wrap].n[2];
                        texture_uvs->n[0] = texture_uvs[-ring_wrap].n[0];
                    }
                    else
                    {
                        texture_uvs->n[0] = (((float)cylinder_index
                                / (float)cylinder_segment_count) * 2.0f);

                        if ( cylinder_index == cylinder_segment_count )
                        {
                            /* close the minor (cylinder) seam: copy this ring's first vertex */
                            points->n[0] = points[-cylinder_segment_count].n[0];
                            points->n[1] = points[-cylinder_segment_count].n[1];
                            points->n[2] = points[-cylinder_segment_count].n[2];
                        }
                        else
                        {
                            points->n[0] = cylinder_cosine_radius;
                            points->n[1] = cylinder_sine_radius;
                            points->n[2] = 0.0f;

                            float cylinder_angle = (((float)cylinder_index
                                    / (float)cylinder_segment_count) * TWO_PI);
                            rotate_vector_about_axis((real_vector3d *)points, &axis,
                                    (float)sin(cylinder_angle), (float)cos(cylinder_angle));

                            float rotated_z = points->n[2];
                            float positioned_y = (points->n[1] + ring_sine_radius);
                            points->n[0] = points->n[0] + ring_cosine_radius;
                            points->n[1] = positioned_y;
                            points->n[2] = rotated_z;
                            matrix4x3_transform_point(matrix, points, points);
                        }
                    }

                    ++points;
                    ++texture_uvs;
                    vertex_count = (__int16)(vertex_count + 1);
                }
            }
        }
    }

    *vertex_count_reference = vertex_count;
    *triangle_strip_count_reference = triangle_strip_count;
}
