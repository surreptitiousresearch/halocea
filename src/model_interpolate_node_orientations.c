/* model_interpolate_node_orientations @0x83798B90 — blend a model's node poses toward `original_node_
 * orientations`, in place, on `target_node_orientations`: each node's rotation/translation/scale becomes
 * original*(1-t) + target_current*t, where t = (frame_index+1)/frame_count. Since target_current is read
 * before being overwritten, repeated calls with the same targets progressively settle them toward the
 * original pose (running exponential-style blend), rather than a one-shot lerp between two fixed poses.
 *
 * DEVIATION: the decompiler drops the real 3rd parameter entirely (target_node_orientations), rendering it
 * as a garbled 64-bit frame_index and a bogus constant pointer passed to quaternions_interpolate_and_
 * normalize; DB's real 5-parameter prototype and disasm confirm target_node_orientations is both q1 and
 * the output of that call (self-mutating in place), and both the blend source and destination for
 * translation/scale. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/real_orientation.h"

#include "headers/real_quaternion.h"
extern void quaternions_interpolate_and_normalize(const real_quaternion *q0, const real_quaternion *q1, float t, real_quaternion *result);

void model_interpolate_node_orientations(model *model, real_orientation *original_node_orientations, real_orientation *target_node_orientations, int16_t frame_index, int16_t frame_count)
{
    float t = (float)(frame_index + 1) / (float)frame_count;
    float complement = 1.0f - t;

    for ( int node_index = 0; node_index < model->nodes.count; node_index++ )
    {
        real_orientation *original = &original_node_orientations[node_index];
        real_orientation *target = &target_node_orientations[node_index];

        target->scale = original->scale * complement + target->scale * t;
        quaternions_interpolate_and_normalize(&original->rotation, &target->rotation, t, &target->rotation);

        target->translation.n[0] = original->translation.n[0] * complement + target->translation.n[0] * t;
        target->translation.n[1] = original->translation.n[1] * complement + target->translation.n[1] * t;
        target->translation.n[2] = original->translation.n[2] * complement + target->translation.n[2] * t;
    }
}
