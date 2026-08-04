/* animation_get_root_matrix @0x837986B8 — the root (node 0) transform of an animation at a frame, as a 4x3
 * matrix built from that node's translation and rotation. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/animation.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_orientation.h"

extern void animation_get_node_orientations(const model *model, const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void matrix4x3_from_point_and_quaternion(real_matrix4x3 *matrix, const real_point3d *point, const real_quaternion *quaternion);

void animation_get_root_matrix(const model *model, const animation *animation, int16_t frame_index, real_matrix4x3 *matrix)
{
    real_orientation node_orientations[64];
    animation_get_node_orientations(model, animation, frame_index, node_orientations);
    matrix4x3_from_point_and_quaternion(matrix, &node_orientations[0].translation, &node_orientations[0].rotation);
}
