/* actor_move_vector_avoidance_find_direction @0x837C8260 — find where a desired direction_vector falls
 * among a circular list of avoidance_directions: for each direction (starting the "previous" cross value
 * from the wrap-around last element), computes cross(direction, direction_vector).x-ish
 * (dir.y*dv.z - dir.z*dv.y) and looks for a sign change from the previous direction's cross value where
 * the dot product dir . direction_vector is still positive (same general hemisphere) — that's the pair of
 * adjacent directions straddling direction_vector. Linearly interpolates both the fractional index and
 * the corresponding avoidance_weights[] value between that pair, weighted by how close each cross value
 * is to zero. Returns false (leaving the outputs unset) if no straddling pair exists (direction_vector
 * doesn't fall between any two consecutive avoidance_directions).
 *
 * DEVIATION: the decompiler fabricates phantom 64-bit types for direction_vector/approximate_weight_reference
 * (splitting them into bogus int/pointer halves); the DB's real 6-parameter prototype — with
 * direction_vector and approximate_weight_reference both used as plain pointers throughout, and
 * avoidance_weights (which the decompiler drops from its own rendering entirely) read via indexed loads
 * at the very end — is ground truth, confirmed via full disasm trace. `previous_index` is read
 * uninitialized if the very first candidate direction already satisfies the straddle test (r31 is
 * genuinely unset on that path in the shipped binary — its own callee-saved value from the caller);
 * reproduced verbatim as an uninitialized local rather than defaulted, matching that edge-case behavior. */

#include <stdint.h>
#include "headers/real_vector3d.h"

uint8_t actor_move_vector_avoidance_find_direction(int16_t direction_count,
    const real_vector3d *avoidance_directions, const float *avoidance_weights, const real_vector3d *direction_vector,
    float *approximate_direction_reference, float *approximate_weight_reference)
{
    if ( direction_count <= 0 )
        return 0;

    int16_t wrap_index = (int16_t)(direction_count - 1);
    float cross_prev = avoidance_directions[wrap_index].n[1] * direction_vector->n[2]
                      - avoidance_directions[wrap_index].n[2] * direction_vector->n[1];

    int16_t previous_index;
    int16_t index = 0;
    float cross_curr;

    while ( 1 )
    {
        const real_vector3d *direction = &avoidance_directions[index];
        cross_curr = direction->n[1] * direction_vector->n[2] - direction->n[2] * direction_vector->n[1];

        float dot = direction->n[2] * direction_vector->n[2] + direction->n[1] * direction_vector->n[1]
                  + direction_vector->n[0] * direction->n[0];

        if ( cross_curr * cross_prev <= 0.0f && dot > 0.0f )
            break;

        cross_prev = cross_curr;
        previous_index = index;
        index = (int16_t)(index + 1);
        if ( index >= direction_count )
            return 0;
    }

    int16_t interpolation_index = index ? index : direction_count;

    *approximate_direction_reference =
        ((float)previous_index * cross_curr - (float)interpolation_index * cross_prev) / (cross_curr - cross_prev);

    *approximate_weight_reference =
        (avoidance_weights[previous_index] * cross_curr - avoidance_weights[interpolation_index] * cross_prev)
        / (cross_curr - cross_prev);

    return 1;
}
