/* compute_acceleration @0x8375E610 — steers current_velocity toward desired_velocity: computes the raw
 * velocity delta (with gravity added to the vertical axis), then re-scales it to a maximum_acceleration/
 * maximum_deceleration limit depending on whether the delta opposes or assists the desired velocity, and
 * finally clamps the result's magnitude to maximum_deceleration.
 *
 * DEVIATION: the decompiler renders this as a 7-parameter function with an unused "acceleration" 5th
 * parameter and phantom unused/output params "a6"/"a7" (writing all results through a7). disasm_range
 * (0x8375E610, 0x8375E660) shows r7 — the first real GPR after the two dead shadow-GPRs reserved for the
 * float maximum_acceleration/maximum_deceleration args — is what's actually written and returned; that is
 * exactly the established 5-arg extern's `acceleration` output parameter used elsewhere. Reconstructed
 * positionally from disasm to match the established signature. */

#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);

real_vector3d *compute_acceleration(const real_vector3d *current_velocity, const real_vector3d *desired_velocity,
        float maximum_acceleration, float maximum_deceleration, real_vector3d *acceleration)
{
    acceleration->n[0] = desired_velocity->n[0] - current_velocity->n[0];
    acceleration->n[1] = desired_velocity->n[1] - current_velocity->n[1];
    acceleration->n[2] = (desired_velocity->n[2] - current_velocity->n[2]) + global_gravity;

    float dot = acceleration->n[0] * desired_velocity->n[0]
              + desired_velocity->n[1] * acceleration->n[1]
              + desired_velocity->n[2] * acceleration->n[2];

    if (dot > 0.000099999997f)
    {
        float delta_length_sq = acceleration->n[0] * acceleration->n[0]
                               + acceleration->n[1] * acceleration->n[1]
                               + acceleration->n[2] * acceleration->n[2];
        float desired_length_sq = desired_velocity->n[0] * desired_velocity->n[0]
                                 + desired_velocity->n[1] * desired_velocity->n[1]
                                 + desired_velocity->n[2] * desired_velocity->n[2];

        /* delta opposes the desired direction less as dot grows; blend toward maximum_acceleration. */
        maximum_deceleration = ((dot * dot / delta_length_sq) / desired_length_sq)
                              * (maximum_acceleration - maximum_deceleration)
                              + maximum_deceleration;
    }

    float length_sq = acceleration->n[0] * acceleration->n[0]
                     + acceleration->n[1] * acceleration->n[1]
                     + acceleration->n[2] * acceleration->n[2];

    if (length_sq > maximum_deceleration * maximum_deceleration)
    {
        float scale = maximum_deceleration / __fsqrts(length_sq);
        acceleration->n[0] *= scale;
        acceleration->n[1] *= scale;
        acceleration->n[2] *= scale;
    }

    return acceleration;
}
