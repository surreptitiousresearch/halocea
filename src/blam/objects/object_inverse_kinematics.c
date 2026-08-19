/* object_inverse_kinematics @0x836EF5B0 — solve a 2-bone inverse-kinematics chain so the "hand" object's
 * named marker reaches the "grip" object's named marker: resolves both markers, walks up the hand model's
 * node hierarchy from the hand marker's node (model.nodes, an array of model_node) to find its parent and
 * grandparent node, then combines the hand marker's inverse node matrix with the grip marker's object-space
 * matrix and hands the result plus the three node indices (grandparent/parent/node) to
 * inverse_kinematics_adjust_matrices to pose `node_matrices`. No-op if either marker can't be resolved, or
 * the node hierarchy doesn't reach two levels up (grandparent index resolves to -1). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/real_matrix4x3.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void inverse_kinematics_adjust_matrices(real_matrix4x3 *desired_hand_matrix, real_matrix4x3 *shoulder_matrix, real_matrix4x3 *elbow_matrix, real_matrix4x3 *hand_matrix);

void object_inverse_kinematics(int hand_object_index, char *hand_marker_name, int grip_object_index,
    char *grip_marker_name, real_matrix4x3 *node_matrices)
{
    char *hand_object = (char *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, hand_object_index)->datum;
    _object_definition *hand_object_definition = TAG_GET(_object_definition, *(int *)hand_object);
    model *hand_model_definition = TAG_GET(model, hand_object_definition->model.index);

    object_marker hand_marker, grip_marker;
    if ( !object_get_marker_by_name(hand_object_index, hand_marker_name, &hand_marker, 1)
      || !object_get_marker_by_name(grip_object_index, grip_marker_name, &grip_marker, 1) )
    {
        return;
    }

    model_node *nodes = (model_node *)hand_model_definition->nodes.address;
    int16_t node_index = hand_marker.node_index;
    int16_t parent_node_index = nodes[node_index].parent_node_index;
    int16_t grandparent_node_index = (parent_node_index == -1) ? -1
        : nodes[parent_node_index].parent_node_index;

    if ( grandparent_node_index == -1 )
        return;

    real_matrix4x3 combined;
    matrix4x3_inverse(&hand_marker.node_matrix, &combined);
    matrix4x3_multiply(&grip_marker.matrix, &combined, &combined);
    inverse_kinematics_adjust_matrices(&combined, &node_matrices[grandparent_node_index],
        &node_matrices[parent_node_index], &node_matrices[node_index]);
}
