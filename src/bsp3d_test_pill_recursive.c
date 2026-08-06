/* bsp3d_test_pill_recursive @0x837E2548 — the core swept-pill sweep over a collision BSP. Recurses the 3D BSP:
 * at an interior node it computes the pill's radius-expanded signed-distance span to the splitting plane and
 * descends the reachable side(s) (front-to-back along the sweep), pushing the crossed plane onto data->plane_stack.
 * At a leaf it walks the leaf's bsp2d_references; for each reference whose plane is on the traversal stack it
 * computes the plane-crossing fraction, projects the sweep into that plane's dominant axis, point-tests the leaf's
 * 2D BSP, runs collision_surface_test_point to record a discrete contact, then runs bsp2d_test_pill_recursive for
 * the swept-pill contact. Visited leaves are appended to result->leaf_indices. Returns TRUE on any contact.
 *
 * Faithful transcription of the decompiler; the dense float expressions and the projection-sign arithmetic are
 * reproduced as emitted. 8*child_index for the leaf record is congruent (mod 2^32) to 8*leaf_index. */

#include <stdint.h>
#include "headers/test_pill_data.h"
#include "headers/collision_leaf.h"
#include "headers/bsp2d_reference.h"
#include "headers/collision_surface.h"
#include "headers/bsp3d_node.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/blam_data_globals.h"


extern float __fabs(float);
extern int bsp2d_test_point(const bsp2d *bsp, const real_point2d *point, int child_index);
extern uint8_t collision_surface_test_point(const collision_bsp *bsp, int16_t breakable_surface_count, const uint8_t *breakable_surface_flags, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *point);
extern uint8_t bsp2d_test_pill_recursive(test_pill_data *data, int child_index);

uint8_t bsp3d_test_pill_recursive(test_pill_data *data, int child_index)
{
    int hit = 0;

    if ( child_index >= 0 )
    {
        const real_point3d  *point  = data->point;
        const real_vector3d *vector = data->vector;
        int   *node = (int *)&((bsp3d_node *)data->bsp->bsp3d.nodes.address)[child_index];
        int    plane_index = *node;
        float  radius_plus = (data->radius + 0.00024414062f);
        float *plane = (float *)&((real_plane3d *)data->bsp->bsp3d.planes.address)[plane_index];

        float along = ((vector->n[0] * plane[0])
                            + ((vector->n[1] * plane[1]) + (vector->n[2] * plane[2])));
        float distance_start = (((plane[0] * point->n[0])
                                             + ((point->n[1] * plane[1]) + (point->n[2] * plane[2])))
                                     - plane[3]);
        float distance_end = (along + distance_start);

        char reaches_back = (distance_start <= radius_plus || distance_end <= radius_plus);
        float radius_minus = (-data->radius - 0.00024414062f);
        uint8_t reaches_front = (distance_start >= radius_minus || distance_end >= radius_minus);

        if ( reaches_back && reaches_front )
        {
            int stack_plane = (along <= 0.0) ? (plane_index & 0x7FFFFFFF) : (plane_index | 0x80000000);
            BOOL positive = along > 0.0;
            data->plane_stack[data->stack_depth++] = stack_plane;
            if ( bsp3d_test_pill_recursive(data, node[(along <= 0.0) + 1]) )
                hit = 1;
            --data->stack_depth;
            if ( bsp3d_test_pill_recursive(data, node[positive + 1]) )
                return 1;
            return hit;
        }
        else if ( bsp3d_test_pill_recursive(data, node[reaches_front + 1]) )
        {
            return 1;
        }
        return hit;
    }

    if ( child_index == -1 )
        return hit;

    int leaf_index = child_index & 0x7FFFFFFF;
    const collision_leaf *leaf = &((const collision_leaf *)data->bsp->leaves.address)[leaf_index];
    int first_reference = leaf->first_bsp2d_reference_index;
    int reference = first_reference;

    if ( first_reference < leaf->bsp2d_reference_count + first_reference )
    {
        do
        {
            const collision_bsp *bsp = data->bsp;
            const bsp2d_reference *reference_entry = &((const bsp2d_reference *)data->bsp->bsp2d_references.address)[reference];
            if ( data->stack_depth > 0 )
            {
                int reference_plane = reference_entry->plane_index;
                int stack_pos = 0;
                while ( data->plane_stack[stack_pos] != reference_plane )
                {
                    stack_pos = (int16_t)(stack_pos + 1);
                    if ( stack_pos >= data->stack_depth )
                        goto next_reference;
                }

                float fraction = 0.0;
                float *plane = (float *)&((real_plane3d *)bsp->bsp3d.planes.address)[reference_plane];
                const real_point3d  *point  = data->point;
                const real_vector3d *vector = data->vector;
                float along = ((plane[0] * vector->n[0])
                                    + ((vector->n[1] * plane[1]) + (vector->n[2] * plane[2])));
                float distance_start = (((plane[0] * point->n[0])
                                                     + ((point->n[1] * plane[1]) + (point->n[2] * plane[2])))
                                             - plane[3]);
                if ( along != 0.0 )
                {
                    float inverse = ((float)1.0 / along);
                    fraction = -((__fabs(inverse) * data->radius)
                                             - -(inverse * distance_start));
                    if ( fraction >= 0.0 )
                    {
                        if ( fraction > 1.0 )
                            fraction = 1.0;
                    }
                    else
                    {
                        fraction = 0.0;
                    }
                }

                if ( data->result->t > fraction )
                {
                    /* dominant axis of the plane normal becomes the projection axis */
                    float ax = __fabs(plane[0]);
                    float ay = __fabs(plane[1]);
                    float az = __fabs(plane[2]);
                    int16_t axis;
                    if ( az < ay || az < ax )
                        axis = (ay >= ax);
                    else
                        axis = 2;
                    data->projection_axis = axis;
                    BOOL plane_positive = plane[axis] > 0.0;
                    const bsp2d *leaf_bsp2d = &bsp->bsp2d;
                    int sign_intermediate = (reference_entry->plane_index < 0) - plane_positive;
                    uint8_t sign =
                        sign_intermediate - (-(reference_entry->plane_index >= 0) - plane_positive + (sign_intermediate == 0));
                    data->projection_sign = sign;

                    /* project the 3D hit point (at `fraction`) onto the plane, then drop to 2D */
                    float hit_y = ((vector->n[1] * fraction) + point->n[1]);
                    float hit_x = ((vector->n[0] * fraction) + point->n[0]);
                    float pushed = -(((plane[0] * ((vector->n[0] * fraction) + point->n[0]))
                                                  + ((plane[2] * ((vector->n[2] * fraction) + point->n[2]))
                                                          + (plane[1] * ((vector->n[1] * fraction) + point->n[1]))))
                                          - plane[3]);
                    float hit_point[3];
                    hit_point[2] = ((plane[2] * pushed)
                                         + ((vector->n[2] * fraction) + point->n[2]));
                    hit_point[1] = ((plane[1] * pushed) + hit_y);
                    hit_point[0] = ((plane[0] * pushed) + hit_x);

                    real_point2d projected;
                    projected.n[0] = hit_point[global_projection3d_mappings[axis][sign][0]];
                    projected.n[1] = hit_point[global_projection3d_mappings[axis][sign][1]];

                    int surface_index = bsp2d_test_point(leaf_bsp2d, &projected, reference_entry->bsp2d_root_index);
                    const collision_bsp *result_bsp = data->bsp;
                    if ( collision_surface_test_point(data->bsp, 0, 0L, surface_index,
                                                      data->projection_axis, data->projection_sign, &projected) )
                    {
                        const collision_surface *surface =
                            &((const collision_surface *)result_bsp->surfaces.address)[surface_index];
                        data->result->t = fraction;
                        collision_bsp_test_pill_result *result = data->result;
                        if ( reference_entry->plane_index >= 0 )
                        {
                            result->plane.normal.n[0] = plane[0];
                            result->plane.normal.n[1] = plane[1];
                            result->plane.normal.n[2] = plane[2];
                            result->plane.distance = plane[3];
                        }
                        else
                        {
                            result->plane.normal.n[0] = -plane[0];
                            result->plane.normal.n[1] = -plane[1];
                            result->plane.normal.n[2] = -plane[2];
                            result->plane.distance = -plane[3];
                        }
                        hit = 1;
                        data->result->surface_index = surface_index;
                        data->result->material_index = surface->material_index;
                    }

                    /* project the start point and the sweep vector into 2D, then sweep-test the leaf bsp2d */
                    const real_point3d *start = data->point;
                    int u_axis = global_projection3d_mappings[data->projection_axis][data->projection_sign][0];
                    int v_axis = global_projection3d_mappings[data->projection_axis][data->projection_sign][1];
                    float start_proj[3];
                    start_proj[0] = (plane[0] * -distance_start) + start->n[0];
                    start_proj[1] = (plane[1] * -distance_start) + start->n[1];
                    start_proj[2] = (plane[2] * -distance_start) + start->n[2];
                    data->p2d.n[0] = start_proj[u_axis];
                    data->p2d.n[1] = start_proj[v_axis];

                    const real_vector3d *sweep = data->vector;
                    float vector_proj[3];
                    vector_proj[0] = (plane[0] * -along) + sweep->n[0];
                    vector_proj[1] = (plane[1] * -along) + sweep->n[1];
                    vector_proj[2] = (plane[2] * -along) + sweep->n[2];
                    data->v2d.n[0] = vector_proj[u_axis];
                    data->v2d.n[1] = vector_proj[v_axis];

                    if ( bsp2d_test_pill_recursive(data, reference_entry->bsp2d_root_index) )
                        hit = 1;
                }
            }
next_reference:
            ++reference;
        }
        while ( reference < leaf->bsp2d_reference_count + leaf->first_bsp2d_reference_index );
    }

    collision_bsp_test_pill_result *result = data->result;
    int leaf_count = result->leaf_count;
    if ( leaf_count >= 256 )
    {
        result->leaf_indices[255] = leaf_index;
    }
    else
    {
        result->leaf_indices[leaf_count] = leaf_index;
        ++data->result->leaf_count;
    }
    return hit;
}
