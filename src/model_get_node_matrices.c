/* model_get_node_matrices @0x83798D28 — build the world-space matrix for every node in a model's
 * skeleton, breadth-first via each node's first_child/next_sibling links (starting from node 0). The
 * root's base matrix is built from (origin, forward, up) — falling back to the world axes/origin for any
 * that are null — then each node's matrix is its parent's matrix times its own default
 * translation/rotation (point_and_quaternion), with the root instead applying its local transform
 * in-place onto its already-built base matrix. node_matrices must have one entry per model node. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_quaternion.h"
#include "headers/blam_data_globals.h"


#include "headers/real_quaternion.h"
extern void matrix4x3_from_point_and_quaternion(real_matrix4x3 *matrix, const real_point3d *point, const real_quaternion *quaternion);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

void model_get_node_matrices(const model *model, real_matrix4x3 *node_matrices, const real_point3d *origin,
    const real_vector3d *forward, const real_vector3d *up)
{
    int16_t queue[120];
    int16_t read_cursor = 0;
    int16_t write_cursor = 1;
    queue[0] = 0;

    do
    {
        int16_t node_index = queue[read_cursor++];
        const model_node *node = (const model_node *)model->nodes.address + node_index;

        real_matrix4x3 local_transform;
        matrix4x3_from_point_and_quaternion(&local_transform, &node->default_translation,
            &node->default_rotation);

        real_matrix4x3 *destination;
        const real_matrix4x3 *parent_matrix;

        if ( node_index )
        {
            destination = &node_matrices[node_index];
            parent_matrix = &node_matrices[node->parent_node_index];
        }
        else
        {
            const real_vector3d *base_up = up ? up : global_up3d;
            const real_vector3d *base_forward = forward ? forward : global_forward3d;
            const real_point3d *base_origin = origin ? origin : global_origin3d;

            matrix4x3_from_point_and_vectors(node_matrices, base_origin, base_forward, base_up);
            destination = node_matrices;
            parent_matrix = node_matrices;
        }

        matrix4x3_multiply(parent_matrix, &local_transform, destination);

        /* recovered: node+0x20/+0x22 are next_sibling/first_child per DB model_node layout
         * (the decompiler's local labels were swapped); order of enqueue is unchanged. */
        uint16_t next_sibling = (uint16_t)node->next_sibling_node_index;
        if ( next_sibling != 0xFFFF )
            queue[write_cursor++] = next_sibling;

        uint16_t first_child = (uint16_t)node->first_child_node_index;
        if ( first_child != 0xFFFF )
            queue[write_cursor++] = first_child;
    }
    while ( read_cursor != write_cursor );
}
