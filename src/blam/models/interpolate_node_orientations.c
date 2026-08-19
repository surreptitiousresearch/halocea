/* interpolate_node_orientations @ 0x83796188 — blend each node's pose from `original` toward `target`
 * by the fraction (frame_index+1)/frame_count, writing the result back into `target`. Scale and
 * translation interpolate linearly; rotation interpolates via quaternions_interpolate_and_normalize.
 * Used to ease an animation from its current frame into the next during playback.
 *
 * The decompiler flags "local variable allocation failed", but the body is well-formed: the 64-bit
 * register pair on frame_count is the long-double ABI carrying frame_index+1 (low) and frame_count
 * (high). DISASM-verified the rotation output is &target[i].rotation (the decompiler aliased it to
 * node 0). */

#include <stdint.h>
#include "headers/real_orientation.h"

#include "headers/real_quaternion.h"
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);

/* disasm: node_count (r3), frame_index (r6), frame_count (r7) all arrive extsh — 16-bit signed */
void interpolate_node_orientations(int16_t node_count, real_orientation *original_node_orientations,
    real_orientation *target_node_orientations, int16_t frame_index, int16_t frame_count)
{
    float t = (float)(frame_index + 1) / (float)frame_count;
    float one_minus_t = 1.0f - t;

    for ( int i = 0; i < node_count; ++i )
    {
        real_orientation *to = &target_node_orientations[i];
        real_orientation *from = &original_node_orientations[i];

        to->scale = from->scale * one_minus_t + to->scale * t;
        quaternions_interpolate_and_normalize(&from->rotation, &to->rotation, t, &to->rotation);
        to->translation.n[0] = from->translation.n[0] * one_minus_t + to->translation.n[0] * t;
        to->translation.n[1] = from->translation.n[1] * one_minus_t + to->translation.n[1] * t;
        to->translation.n[2] = from->translation.n[2] * one_minus_t + to->translation.n[2] * t;
    }
}
