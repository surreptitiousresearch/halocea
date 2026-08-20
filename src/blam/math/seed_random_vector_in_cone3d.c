/* seed_random_vector_in_cone3d @0x83723268 — pick a random unit-ish direction within a cone around
 * `axis`, between inner_cone_angle and outer_cone_angle. Picks a random table direction, forms a vector
 * perpendicular to the axis (axis x random), and rotates the axis about that perpendicular by a random
 * angle in [inner, outer]. Result is written to `result` and returned.
 *
 * NOTE: the decompiler reported "local variable allocation failed" for this function. The output target,
 * the inner-angle lower bound, and the cos/sin-of-angle arguments to rotate_vector_about_axis were
 * recovered from the disassembly (both trig args are of the sampled angle, not the cone bound). */

#include <stdint.h>
#include "headers/random_math_globals.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/ppc_intrinsics.h"


extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern double cos(double x);
extern double sin(double x);

real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis,
        float inner_cone_angle, float outer_cone_angle, real_vector3d *result)
{
    unsigned int next_seed = 1664525 * *seed + 1013904223;
    real_vector3d *random_direction_table = random_math_globals.random_direction_table;
    int random_direction_table_size = random_math_globals.random_direction_table_size;
    float *random_direction;
    real_vector3d perpendicular;
    float cross_x, cross_y, cross_z;
    float length;

    *result = *axis;
    *seed = next_seed;

    random_direction = random_direction_table[(int16_t)((unsigned int)(((uint16_t)(next_seed >> 16)) * random_direction_table_size) >> 16)].n;

    cross_x = (axis->n[1] * random_direction[2]) - (axis->n[2] * random_direction[1]);
    cross_y = (axis->n[2] * random_direction[0]) - (axis->n[0] * random_direction[2]);
    cross_z = (axis->n[0] * random_direction[1]) - (axis->n[1] * random_direction[0]);
    length = __fsqrts(((cross_z * cross_z) + ((cross_y * cross_y) + (cross_x * cross_x))));

    if ( __fabs(length) >= 0.000099999997 )
    {
        float inverse_length = 1.0f / length;
        perpendicular.n[0] = inverse_length * cross_x;
        perpendicular.n[1] = cross_y * inverse_length;
        perpendicular.n[2] = cross_z * inverse_length;

        if ( length > 0.0 )
        {
            unsigned int angle_seed = 1664525 * next_seed + 1013904223;
            float fraction = (float)(uint16_t)(angle_seed >> 16) * 0.000015259022f;   /* in [0,1) */
            double angle;
            *seed = angle_seed;
            angle = ((fraction * (outer_cone_angle - inner_cone_angle)) + inner_cone_angle);
            rotate_vector_about_axis(result, &perpendicular, (float)sin(angle), (float)cos(angle));
        }
    }
    return result;
}
