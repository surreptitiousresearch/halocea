/* get_shake_matrix @0x83737E78 — build a random shake matrix: a random-axis rotation scaled by
 * `rotation_magnitude` (radians) if nonzero, plus a random-direction translation scaled by
 * `translation_magnitude` if nonzero. Either or both parts are skipped when their magnitude is 0.0.
 *
 * DEVIATION: the decompiler's own inferred prototype carries two phantom trailing parameters and
 * types the first two as doubles; the DB's real (and disasm-confirmed) prototype is 3 args
 * (translation_magnitude, rotation_magnitude as floats in f1/f2, shake_matrix in r5 — r3/r4 are dead
 * GPR shadow slots for the two float args). Flagged "local variable allocation has failed" by the
 * decompiler; every field offset below was re-derived from disassembly. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern double sin(double x);
extern double cos(double x);
extern void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine);

void get_shake_matrix(float translation_magnitude, float rotation_magnitude, real_matrix4x3 *shake_matrix)
{
    if (rotation_magnitude != 0.0f)
    {
        float sine = (float)sin(rotation_magnitude);
        float cosine = (float)cos(rotation_magnitude);

        real_vector3d axis;
        seed_random_direction3d(get_global_local_random_seed_address(), &axis);
        matrix4x3_rotation_from_axis_and_angle(shake_matrix, &axis, sine, cosine);
    }

    if (translation_magnitude != 0.0f)
    {
        real_vector3d direction;
        seed_random_direction3d(get_global_local_random_seed_address(), &direction);
        shake_matrix->n[3][0] = direction.n[0] * translation_magnitude;
        shake_matrix->n[3][1] = direction.n[1] * translation_magnitude;
        shake_matrix->n[3][2] = direction.n[2] * translation_magnitude;
    }
}
