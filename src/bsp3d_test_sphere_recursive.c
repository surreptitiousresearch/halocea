/* bsp3d_test_sphere_recursive @0x837E1F40 — recurse the collision 3D BSP gathering everything a query sphere
 * overlaps. Descends a single side while the sphere stays clear of the splitting plane; when the sphere straddles
 * a plane it recurses both children (pushing the plane on the traversal stack). At each leaf it appends the leaf
 * index and, for every bsp2d_reference whose plane is on the stack, projects the sphere centre into that plane's
 * dominant axis and runs bsp2d_test_sphere_recursive. Faithful transcription. */

#include <stdint.h>
#include "headers/test_sphere_data.h"
#include "headers/collision_leaf.h"
#include "headers/bsp2d_reference.h"
#include "headers/bsp3d_node.h"
#include "headers/real_plane3d.h"
#include "headers/real_point2d.h"
#include "headers/blam_data_globals.h"

extern float __fabs(float);
extern void bsp2d_test_sphere_recursive(test_sphere_data *data, int child_index);

void bsp3d_test_sphere_recursive(test_sphere_data *data, int child_index)
{
    int *node;

    if ( child_index >= 0 )
    {
        const real_point3d *center = data->center;
        float radius = data->radius;
        float negative_radius = -radius;
        float cx = center->n[0];
        float cy = center->n[1];
        float cz = center->n[2];

        while ( 1 )
        {
            node = (int *)&((bsp3d_node *)data->bsp->bsp3d.nodes.address)[child_index];
            float *plane = (float *)&((real_plane3d *)data->bsp->bsp3d.planes.address)[*node];
            float distance = (((plane[0] * cx)
                                           + ((plane[1] * cy) + (plane[2] * cz)))
                                   - plane[3]);
            if ( distance > negative_radius && distance < radius )
                break;                                   /* sphere straddles this plane */
            child_index = node[(distance > negative_radius) + 1];
            if ( child_index < 0 )
                goto leaf;                               /* descended into a leaf */
        }

        data->plane_stack[data->stack_depth++] = *node | 0x80000000;
        bsp3d_test_sphere_recursive(data, node[1]);
        int depth = data->stack_depth - 1;
        data->stack_depth = depth;
        data->plane_stack[depth] = *node & 0x7FFFFFFF;
        ++data->stack_depth;
        bsp3d_test_sphere_recursive(data, node[2]);
        --data->stack_depth;
        return;
    }

leaf:
    if ( child_index == -1 )
        return;

    collision_bsp_test_sphere_result *result = data->result;
    const collision_leaf *leaf = &((const collision_leaf *)data->bsp->leaves.address)[child_index & 0x7FFFFFFF];
    int leaf_count = result->leaf_count;
    if ( leaf_count < 256 )
    {
        result->leaf_indices[leaf_count] = child_index & 0x7FFFFFFF;
        ++data->result->leaf_count;
    }

    int first_reference = leaf->first_bsp2d_reference_index;
    int reference = first_reference;
    if ( first_reference < leaf->bsp2d_reference_count + first_reference )
    {
        do
        {
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

                float *plane = (float *)&((real_plane3d *)data->bsp->bsp3d.planes.address)[reference_plane];
                const real_point3d *center = data->center;
                float ax = __fabs(plane[0]);
                float ay = __fabs(plane[1]);
                float az = __fabs(plane[2]);
                float pushed = -(((plane[0] * center->n[0])
                                              + ((center->n[1] * plane[1]) + (center->n[2] * plane[2])))
                                      - plane[3]);
                float projected[3];
                projected[0] = (plane[0] * pushed) + center->n[0];
                projected[1] = (plane[1] * pushed) + center->n[1];
                projected[2] = (plane[2] * pushed) + center->n[2];

                int16_t axis;
                if ( az < ay || az < ax )
                    axis = (ay >= ax);
                else
                    axis = 2;
                data->projection_axis = axis;
                BOOL plane_positive = plane[axis] > 0.0;
                int sign_intermediate = (reference_entry->plane_index < 0) - plane_positive;
                uint8_t sign =
                    sign_intermediate - (-(reference_entry->plane_index >= 0) - plane_positive + (sign_intermediate == 0));
                data->projection_sign = sign;
                data->center2d.n[0] = projected[global_projection3d_mappings[axis][sign][0]];
                data->center2d.n[1] = projected[global_projection3d_mappings[axis][sign][1]];
                bsp2d_test_sphere_recursive(data, reference_entry->bsp2d_root_index);
            }
next_reference:
            ++reference;
        }
        while ( reference < leaf->bsp2d_reference_count + leaf->first_bsp2d_reference_index );
    }
}
