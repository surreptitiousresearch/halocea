/* model_node_matrices_from_orientations @0x83798E68 — BFS the model's node tree turning each node's local
 * orientation into a world-space matrix. The root node's parent is a synthetic basis built from
 * origin/forward/up; every other node's parent is its own already-computed matrix in `node_matrices`. */

#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/real_orientation.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_from_orientation(real_matrix4x3 *matrix, const real_orientation *orientation);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

void model_node_matrices_from_orientations(const model *model, real_matrix4x3 *node_matrices,
                                            const real_orientation *node_orientations, const real_point3d *origin,
                                            const real_vector3d *forward, const real_vector3d *up)
{
    real_matrix4x3 root_matrix;
    matrix4x3_from_point_and_vectors(&root_matrix, origin, forward, up);

    if ( model->nodes.count <= 0 )
        return;

    unsigned __int16 node_stack[64];
    __int16 head = 0;
    __int16 tail = 1;
    node_stack[0] = 0;

    do
    {
        __int16 current = node_stack[head++];
        model_node *node = &((model_node *)model->nodes.address)[current];

        const real_matrix4x3 *parent_matrix = current ? &node_matrices[node->parent_node_index] : &root_matrix;

        real_matrix4x3 local_matrix;
        matrix4x3_from_orientation(&local_matrix, &node_orientations[current]);
        matrix4x3_multiply(parent_matrix, &local_matrix, &node_matrices[current]);

        if ( node->next_sibling_node_index != 0xFFFF )
            node_stack[tail++] = node->next_sibling_node_index;
        if ( node->first_child_node_index != 0xFFFF )
            node_stack[tail++] = node->first_child_node_index;
    }
    while ( head != tail );
}
