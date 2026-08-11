/* effect_random_translational_velocity @0x836E1320 — pick a random launch direction and speed for an
 * emitted particle. The speed magnitude is a scaled random range; the direction starts along `forward`
 * and is perturbed within a random cone (cone_angle, optionally scaled by the effect drivers). The
 * resulting unit direction and the direction*magnitude velocity are written out.
 *
 * Per the disassembly: the speed range uses scale bits at index 0 (magnitude) of the a/b masks; the cone
 * angle scales on bit 2 (0x4). The two unused leading parameters of effect_real_random_range receive
 * register garbage in the original and are passed as 0 here. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/effect_velocity_scaleable_values_flags.h"
#include "headers/real_vector3d.h"

extern float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound, float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t first_bit_index);
extern float real_seed_random(uint32_t *seed);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern double cos(double x);
extern double sin(double x);

extern float effect_scale(const effect_datum *effect, float value, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t bit_index);
void effect_random_translational_velocity(uint32_t *seed, const effect_datum *effect,
        const real_vector3d *forward, real_vector3d *direction, real_vector3d *velocity, float lower_bound,
        float upper_bound, float cone_angle, unsigned int a_scale_flags, unsigned int b_scale_flags)
{
    float magnitude = effect_real_random_range(seed, effect, lower_bound, upper_bound,
                                               a_scale_flags, b_scale_flags, 0);
    double angle;

    /* DEVIATION: collapsed to effect_scale@0x836E1210 (donor is zero-xref, inlined at this site); bit_index folds from _effect_velocity_cone_bit (=2) */
    cone_angle = effect_scale(effect, cone_angle, a_scale_flags, b_scale_flags, _effect_velocity_cone_bit);

    angle = (real_seed_random(seed) * cone_angle);

    direction->n[0] = forward->n[0];
    direction->n[1] = forward->n[1];
    direction->n[2] = forward->n[2];

    if ( angle != 0.0 )
    {
        real_vector3d axis[2];
        seed_random_direction3d(seed, axis);
        rotate_vector_about_axis(direction, axis, (float)sin(angle), (float)cos(angle));
    }

    velocity->n[0] = direction->n[0] * magnitude;
    velocity->n[1] = direction->n[1] * magnitude;
    velocity->n[2] = direction->n[2] * magnitude;
}
