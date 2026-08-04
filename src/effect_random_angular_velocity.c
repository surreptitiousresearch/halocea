/* effect_random_angular_velocity @0x836E1440 — produce a random angular-velocity vector for an emitted
 * particle. A scaled random magnitude is drawn; if it is zero the result is the zero vector, otherwise
 * a random unit direction is chosen and scaled by the magnitude.
 *
 * DEVIATION: the decompiler under-counted the effect_real_random_range call (7 visible args vs the
 * canonical 9-arg prototype). Per the call-site disassembly the register plumbing is: unused_a = result
 * pointer, unused_b = a_scale_flags, scale_a_flags = scale_a_flags, first_bit_index = constant 3; the scale_b_flags
 * slot (r8) is loaded from an undefined register and is passed as 0 here. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound, float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t first_bit_index);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

void effect_random_angular_velocity(unsigned int *seed, const effect_datum *effect, real_vector3d *result, float lower_bound, float upper_bound, unsigned int a_scale_flags, unsigned int b_scale_flags, int16_t scale_a_flags)
{
    float magnitude = effect_real_random_range(seed, effect, lower_bound, upper_bound,
            scale_a_flags, 0, 3);

    if ( magnitude == 0.0f )
    {
        *result = *global_zero_vector3d;
    }
    else
    {
        seed_random_direction3d(seed, result);
        result->n[0] = result->n[0] * magnitude;
        result->n[1] = magnitude * result->n[1];
        result->n[2] = magnitude * result->n[2];
    }
}
