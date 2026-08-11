/* animation_graph_node_matrices_from_orientations @0x83796060 — BFS an animation graph's node tree turning
 * each node's local orientation into a world-space matrix. Same shape as the sibling
 * model_node_matrices_from_orientations, but walks animation_graph_node instead of model_node. */

#include <stdint.h>
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

    uint16_t node_stack[64];
    int16_t head = 0;
    int16_t tail = 1;
    node_stack[0] = 0;

    do
    {
        int16_t current = node_stack[head++];
        animation_graph_node *node = &((animation_graph_node *)graph->nodes.address)[current];

        const real_matrix4x3 *parent_matrix = current ? &node_matrices[node->parent_node_index] : &root_matrix;

        real_matrix4x3 local_matrix;
        matrix4x3_from_orientation(&local_matrix, &node_orientations[current]);
        matrix4x3_multiply(parent_matrix, &local_matrix, &node_matrices[current]);

        if ( (uint16_t)node->next_sibling_node_index != 0xFFFF )   /* DEVIATION: int16_t field — uncast, the promoted -1 made this always TRUE and -1 was pushed onto the walk stack; binary zero-extends, lhz r11,0x20(r30) @0x83796120 + cmplwi cr6,r11,0xFFFF @0x83796124 */
            node_stack[tail++] = node->next_sibling_node_index;
        if ( (uint16_t)node->first_child_node_index != 0xFFFF )   /* DEVIATION: same shape — lhz r10,0x22(r30) @0x83796144 + cmplwi cr6,r10,0xFFFF @0x83796148 */
            node_stack[tail++] = node->first_child_node_index;
    }
    while ( head != tail );
}
