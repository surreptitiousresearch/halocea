/* actor_move_vector_avoidance_find_direction @0x837C8260 — find where a desired direction_vector falls
 * among a circular list of avoidance_directions: for each direction (starting both the "previous" index
 * and its cross value from the wrap-around last element), computes cross(direction, direction_vector).x-ish
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
 * at the very end — is ground truth, confirmed via full disasm trace (the caller sets r5 = &weights[0],
 * r6 = &direction_vector at 0x837C9A20-0x837C9A34).
 *
 * DEVIATION: the wrap-around index and the running "previous index" are one register, not two —
 * 0x837C8280 `extsh r31, r11` seeds r31 = direction_count - 1 before the loop and the continue path
 * advances it with `mr r31, r30` (0x837C8320), so the index == 0 exit yields the pair
 * (direction_count - 1, 0) rather than an uninitialized previous index.
 *
 * DEVIATION: on that exit direction_count substitutes for the *numeric* interpolation coordinate only —
 * 0x837C8340-0x837C8348 skips `mr r9, r11`, leaving r9 = direction_count for the fcfid/frsp coordinate
 * math. The weight loads keep the raw loop index (0x837C8378 `slwi r11, r11, 2`, 0x837C839C
 * `lfsx f1, r11, r5`), so the wrap path reads avoidance_weights[0]; the binary never forms the address
 * of avoidance_weights[direction_count]. */

#include <stdint.h>
#include "headers/real_vector3d.h"

uint8_t actor_move_vector_avoidance_find_direction(int16_t direction_count,
    const real_vector3d *avoidance_directions, const float *avoidance_weights, const real_vector3d *direction_vector,
    float *approximate_direction_reference, float *approximate_weight_reference)
{
    if ( direction_count <= 0 )
        return 0;

    int16_t previous_index = (int16_t)(direction_count - 1);
    float cross_prev = avoidance_directions[previous_index].n[1] * direction_vector->n[2]
                      - avoidance_directions[previous_index].n[2] * direction_vector->n[1];

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

    int16_t interpolation_coordinate = index ? index : direction_count;

    *approximate_direction_reference =
        ((float)previous_index * cross_curr - (float)interpolation_coordinate * cross_prev) / (cross_curr - cross_prev);

    *approximate_weight_reference =
        (avoidance_weights[previous_index] * cross_curr - avoidance_weights[index] * cross_prev)
        / (cross_curr - cross_prev);

    return 1;
}
