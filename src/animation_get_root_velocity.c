/* animation_get_root_velocity @0x83798714 — root-bone (node 0) velocity between consecutive animation
 * frames: the translation delta from frame_index-1 to frame_index (frame 0 treated as frame 1). */

#include <stdint.h>
#include "headers/model.h"
#include "headers/animation.h"
#include "headers/real_orientation.h"
#include "headers/real_vector3d.h"

extern void animation_get_node_orientations(const model *model, const animation *animation, int16_t frame_index, real_orientation *node_orientations);

void animation_get_root_velocity(const model *model, const animation *animation, int16_t frame_index,
    real_vector3d *velocity)
{
    int16_t frame = frame_index ? frame_index : 1;

    real_orientation current;
    real_orientation previous;
    animation_get_node_orientations(model, animation, frame, &current);
    animation_get_node_orientations(model, animation, frame - 1, &previous);

    velocity->n[0] = current.translation.n[0] - previous.translation.n[0];
    velocity->n[1] = current.translation.n[1] - previous.translation.n[1];
    velocity->n[2] = current.translation.n[2] - previous.translation.n[2];
}
