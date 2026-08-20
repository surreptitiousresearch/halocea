/* projectile_aim_ballistic @0x837592A8 — solve the launch direction for a projectile fired from `origin` that
 * must strike `target_point` under gravity. Given a launch speed (base_velocity, optionally overridden by
 * *forced_velocity and/or capped by a minimum-ballistic-fraction term), it computes the time of flight and the
 * unit aim vector, choosing the low arc (lob == 0) or high arc (lob != 0). Returns 1 when a valid arc was found,
 * 0 when it fell back to a best-effort / degenerate aim. The various result_* out-params receive the launch
 * speed, time of flight, range, and the pre-normalized vertical / horizontal velocity components.
 *
 * DEVIATION (FPR-shadow disaster): the two leading float args (base_velocity in f1, gravity_scale in f2) shadow
 * GPR slots r3/r4, so every pointer arg shifts down: origin=r5, target_point=r6, target_velocity_min=r7 (loaded
 * but never used), target_ballistic_fraction_min=r8, forced_velocity=r9, lob=r10, then six stack out-params.
 * The decompiler mis-parsed this completely — the DB prototype at 0x837592A8 keeps origin and target_point
 * as separate pointers but fuses target_velocity_min into an __int64 target_ballistic_fraction_min,
 * invents ~25 phantom args, and scrambles the result stores. (The origin/target_point pair-fusion belongs
 * to projectile_aim @0x83759680, whose prototype carries the __int64 target_point.) This was reconstructed
 * from the disassembly against the DB prototype. fcfid/__fsqrts/fsel/fabs sequences are restored as plain
 * casts / intrinsics. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);
extern double __fabs(double x);

extern float normalize3d(real_vector3d *v);

uint8_t projectile_aim_ballistic(float base_velocity, float gravity_scale,
                                         const real_point3d *origin, const real_point3d *target_point,
                                         float *target_velocity_min, float *target_ballistic_fraction_min,
                                         float *forced_velocity, uint8_t lob,
                                         real_vector3d *result_aim_vector, float *result_velocity,
                                         float *result_ticks, float *result_distance,
                                         float *result_vertical_velocity, float *result_horizontal_velocity)
{
    (void)target_velocity_min;  /* arrives in r7 but is never referenced by this routine */

    real_vector3d delta;
    delta.__s1.i = target_point->n[0] - origin->n[0];
    delta.__s1.j = target_point->n[1] - origin->n[1];
    delta.__s1.k = target_point->n[2] - origin->n[2];

    float gravity_raw = global_gravity * gravity_scale;
    float gravity = (gravity_raw >= 0.0f) ? gravity_raw : 0.0f;  /* fsel f13, f7, f7, 0.0 */

    float distance_sq = delta.__s1.i * delta.__s1.i + delta.__s1.j * delta.__s1.j + delta.__s1.k * delta.__s1.k;
    float gravity_z = gravity * delta.__s1.k;
    float quarter_gravity_sq = (gravity * gravity) * 0.25f;
    float half_gravity_sq = quarter_gravity_sq * 2.0f;
    float distance_sq_gravity_sq = (distance_sq * quarter_gravity_sq) * 4.0f;  /* == distance_sq * gravity^2 */
    float distance_times_gravity = __fsqrts(distance_sq_gravity_sq);
    float min_velocity = __fsqrts(distance_times_gravity / half_gravity_sq);   /* == sqrt(2*distance/gravity) */
    float arc_base = gravity_z + distance_times_gravity;
    float velocity_floor = (arc_base >= 0.0f) ? __fsqrts(arc_base) : 0.0f;

    /* pick the launch speed */
    float effective_velocity;
    if (forced_velocity)
    {
        effective_velocity = *forced_velocity;
    }
    else
    {
        effective_velocity = base_velocity;
        if (target_ballistic_fraction_min)
        {
            float fraction = *target_ballistic_fraction_min;
            if (fraction > 0.0f)
            {
                float lob_velocity = fraction * min_velocity;
                float lob_velocity_sq = lob_velocity * lob_velocity;
                float cap_inner = gravity_z + lob_velocity_sq * quarter_gravity_sq + distance_sq / lob_velocity_sq;
                float cap_velocity = __fsqrts(cap_inner);
                if (base_velocity > cap_velocity)
                    effective_velocity = cap_velocity;
            }
        }
    }

    /* solve for the time-of-flight factor of the chosen arc */
    uint8_t result_flag = 1;
    uint8_t solved = 0;
    float time_of_flight;

    if (effective_velocity >= velocity_floor)
    {
        float neg_discriminant_base = gravity_z - effective_velocity * effective_velocity;
        float discriminant = neg_discriminant_base * neg_discriminant_base - distance_sq_gravity_sq;
        if (neg_discriminant_base < 0.0f && discriminant >= 0.0f)
        {
            float sign = lob ? 1.0f : -1.0f;  /* high arc (+) vs low arc (-) */
            float numerator = __fsqrts(discriminant) * sign - neg_discriminant_base;
            float time_sq = numerator / half_gravity_sq;
            if (time_sq > 0.0f)
            {
                solved = 1;
                time_of_flight = __fsqrts(time_sq);
            }
        }
    }

    if (!solved)
    {
        result_flag = 0;
        time_of_flight = min_velocity;
        effective_velocity = velocity_floor;
    }

    /* build the launch velocity vector that reaches the target in time_of_flight */
    float inverse_time = 1.0f / time_of_flight;
    float range = time_of_flight * effective_velocity;
    float aim_x = inverse_time * delta.__s1.i;
    float aim_y = inverse_time * delta.__s1.j;
    float aim_z = inverse_time * delta.__s1.k + (time_of_flight * gravity) * 0.5f;

    float horizontal_speed = __fsqrts(aim_x * aim_x + aim_y * aim_y);
    float total_speed = __fsqrts((aim_z * aim_z + aim_x * aim_x) + aim_y * aim_y);

    real_vector3d aim;
    uint8_t degenerate;
    if (__fabs(total_speed) >= 0.000099999997f)
    {
        float inverse_total = 1.0f / total_speed;
        aim.__s1.i = inverse_total * aim_x;
        aim.__s1.j = inverse_total * aim_y;
        aim.__s1.k = inverse_total * aim_z;
        degenerate = (total_speed == 0.0f);
    }
    else
    {
        degenerate = 1;
    }

    if (degenerate)
    {
        aim = delta;
        result_flag = 0;
        if (normalize3d(&aim) == 0.0f)
            aim = *global_up3d;
    }

    *result_aim_vector = aim;
    if (result_distance)
        *result_distance = range;
    if (result_velocity)
        *result_velocity = effective_velocity;
    if (result_vertical_velocity)
        *result_vertical_velocity = aim_z;
    if (result_horizontal_velocity)
        *result_horizontal_velocity = horizontal_speed;
    if (result_ticks)
        *result_ticks = time_of_flight;

    return result_flag;
}
