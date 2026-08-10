/* animation_get_root_velocity @0x837986F8 — root-bone (node 0) velocity between consecutive animation
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

    /* DEVIATION: both pose buffers were reconstructed as single real_orientation objects, but the
     * callee writes one 32-byte real_orientation per node — animation_get_node_orientations
     * @0x83796B48 loops to animation->node_count (lhz r11, 0x2C(r30) @0x83796C0C) and forms each
     * element as out_ptr + (node_index << 5) (slwi r11, r27, 5 @0x83796C58) — so a scalar is
     * overrun by every model with more than one node. The frame (stwu 0x1080) passes r1+0x50 to
     * the first call and r1+0x850 to the second (addi r6, r1, 0x1080+var_1030 / +var_830), and the
     * saved-GPR block sits at r1+0x1060:
     *   0x850  - 0x50  = 0x800 = 2048 = 64 * sizeof(real_orientation) (32)
     *   0x1060 - 0x850 = 0x810 -> the second buffer holds the same 0x800 extent
     * so both locals are 64-element arrays, and the root node is element 0 (the six loads are at
     * +0x60/+0x64/+0x68 and +0x860/+0x864/+0x868, i.e. buffer_base + offsetof(translation)).
     * 64 is the binary's own ceiling, not an assumption: the callee selects each node's channel
     * bits with lwzx at 4*(node_index >> 5) + 0x5C / 0x6C / 0x7C (@0x83796C6C-0x83796C98) over the
     * animation's three 2-word flag arrays = 2 * 32 bits = 64 nodes. Sibling
     * animation_get_root_matrix @0x837986B8 reserves exactly the same 0x800 extent (frame 0x860,
     * buffer r1+0x50, saved regs at r1+0x850) and is already declared real_orientation[64]. */
    real_orientation current[64];
    real_orientation previous[64];
    animation_get_node_orientations(model, animation, frame, current);
    animation_get_node_orientations(model, animation, frame - 1, previous);

    velocity->n[0] = current[0].translation.n[0] - previous[0].translation.n[0];
    velocity->n[1] = current[0].translation.n[1] - previous[0].translation.n[1];
    velocity->n[2] = current[0].translation.n[2] - previous[0].translation.n[2];
}
