/* animation_graph_node_matrices_from_orientations @0x83796060 — BFS an animation graph's node tree turning
 * each node's local orientation into a world-space matrix. Same shape as the sibling
 * model_node_matrices_from_orientations, but walks animation_graph_node instead of model_node. */

#include "headers/animation_graph.h"
#include "headers/animation_graph_node.h"
#include "headers/global_tag_instances.h"
#include "headers/real_orientation.h"
#include "headers/real_matrix4x3.h"

extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_from_orientation(real_matrix4x3 *matrix, const real_orientation *orientation);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

void animation_graph_node_matrices_from_orientations(int animation_graph_index, real_matrix4x3 *node_matrices,
                                                      const real_orientation *node_orientations,
                                                      const real_point3d *origin, const real_vector3d *forward,
                                                      const real_vector3d *up)
{
    const animation_graph *graph = TAG_GET(const animation_graph, animation_graph_index);

    real_matrix4x3 root_matrix;
    matrix4x3_from_point_and_vectors(&root_matrix, origin, forward, up);

    if ( graph->nodes.count <= 0 )
        return;

    unsigned __int16 node_stack[64];
    __int16 head = 0;
    __int16 tail = 1;
    node_stack[0] = 0;

    do
    {
        __int16 current = node_stack[head++];
        animation_graph_node *node = &((animation_graph_node *)graph->nodes.address)[current];

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
