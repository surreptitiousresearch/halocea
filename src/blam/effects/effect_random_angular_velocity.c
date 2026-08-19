/* effect_random_angular_velocity @0x836E1440 — produce a random angular-velocity vector for an emitted
 * particle. A scaled random magnitude is drawn; if it is zero the result is the zero vector, otherwise
 * a random unit direction is chosen and scaled by the magnitude.
 *
 * DEVIATION: the call to effect_real_random_range forwards BOTH scale-flag parameters. The
 * disassembly is `mr r7, r8` / `mr r8, r9` / `li r9, 3` at 0x836E1458-0x836E1460, i.e. the callee's
 * a_scale_flags <- our r8 (a_scale_flags), its b_scale_flags <- our r9 (b_scale_flags), and
 * first_bit_index is the constant 3. The earlier reading passed the trailing int16_t parameter and a
 * literal 0 instead; it came from assuming the two float parameters consume no GPR slot. They do —
 * lower_bound/upper_bound occupy slots 3 and 4 (r6/r7) while living in f1/f2. The trailing int16_t
 * (r10) is genuinely never read by the binary and the DB prototype leaves it unnamed. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound, float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t first_bit_index);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);

void effect_random_angular_velocity(unsigned int *seed, const effect_datum *effect, real_vector3d *result, float lower_bound, float upper_bound, unsigned int a_scale_flags, unsigned int b_scale_flags, int16_t unused_first_bit_index)
{
    float magnitude = effect_real_random_range(seed, effect, lower_bound, upper_bound,
            a_scale_flags, b_scale_flags, 3);

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
