/* actor_look_find_random_vector @0x837FB358 — pick a random look direction for an actor by perturbing a
 * base direction within yaw/pitch ranges. Builds a horizontal pitch axis perpendicular to the base
 * direction (falling back to global left when the base is near-vertical), then repeatedly: draws random
 * yaw and pitch angles, rotates the base vector by pitch about that axis and by yaw about up, and — when
 * avoid_obstructions is set — rejects the candidate if a short collision probe is blocked. Up to 10
 * attempts; on success the normalized direction is written to result_vector and 1 is returned, otherwise 0.
 *
 * DEVIATION: this is a PPC FPR-shadow case. The four float angle args (yaw/pitch min/max) each reserve a
 * positional GPR slot, so the real result_vector pointer arrives in r10 — which the decompiler mislabeled
 * as a phantom trailing arg. Per the DB prototype it is the 8th parameter, result_vector. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"
#include "headers/ppc_intrinsics.h"


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern double sin(double x);
extern double cos(double x);

uint8_t actor_look_find_random_vector(const real_point3d *center_point, const real_vector3d *base_vector, uint8_t avoid_obstructions, float yaw_min, float yaw_max, float pitch_min, float pitch_max, real_vector3d *result_vector)
{
    /* Horizontal axis perpendicular to the base direction, used for pitch rotation. */
    real_vector3d pitch_axis;
    pitch_axis.n[0] = -base_vector->n[1];
    pitch_axis.n[1] = base_vector->n[0];
    pitch_axis.n[2] = 0.0f;
    float axis_length = __fsqrts(base_vector->n[0] * base_vector->n[0]
                      + (-base_vector->n[1]) * (-base_vector->n[1]));

    if (__fabs(axis_length) < 0.000099999997f)
    {
        pitch_axis = *global_left3d;
    }
    else
    {
        float inverse_length = 1.0f / axis_length;
        pitch_axis.n[0] = inverse_length * (-base_vector->n[1]);
        pitch_axis.n[1] = base_vector->n[0] * inverse_length;
        pitch_axis.n[2] = inverse_length * 0.0f;
    }

    int attempts = 0;
    real_vector3d candidate;
    while (1)
    {
        float yaw = real_seed_random_range(get_global_random_seed_address(), yaw_min, yaw_max);
        float pitch = real_seed_random_range(get_global_random_seed_address(), pitch_min, pitch_max);

        candidate.n[0] = base_vector->n[0];
        candidate.n[1] = base_vector->n[1];
        candidate.n[2] = base_vector->n[2];
        rotate_vector_about_axis(&candidate, &pitch_axis, (float)sin(pitch), (float)cos(pitch));
        rotate_vector_about_axis(&candidate, global_up3d, (float)sin(yaw), (float)cos(yaw));

        uint8_t unobstructed = 1;
        if (avoid_obstructions)
        {
            real_vector3d probe;
            probe.n[0] = candidate.n[0] * 3.0f;
            probe.n[1] = candidate.n[1] * 3.0f;
            probe.n[2] = candidate.n[2] * 3.0f;
            collision_result collision[2];
            unobstructed = collision_test_vector((1u << _collision_test_front_facing_surfaces_bit) | (1u << _collision_test_structure_bit), center_point, &probe, -1, collision) == 0;
        }

        if (unobstructed)
            break;

        attempts = (int16_t)(attempts + 1);
        if (attempts >= 10)
            return 0;
    }

    float length = __fsqrts(candidate.n[0] * candidate.n[0]
                 + (candidate.n[2] * candidate.n[2] + candidate.n[1] * candidate.n[1]));
    if (__fabs(length) >= 0.000099999997f)
    {
        float inverse_length = 1.0f / length;
        candidate.n[0] = inverse_length * candidate.n[0];
        candidate.n[1] = candidate.n[1] * inverse_length;
        candidate.n[2] = candidate.n[2] * inverse_length;
    }

    result_vector->n[0] = candidate.n[0];
    result_vector->n[1] = candidate.n[1];
    result_vector->n[2] = candidate.n[2];
    return 1;
}
