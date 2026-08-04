/* unit_running_blind @ 0x836CD620 — compute the "running blind" facing vector for a unit being driven
 * by an actor that cannot see (e.g. fleeing). Tries the actor's running-blind vector (actor+500 holds the
 * actor index); if none, falls back to global_forward3d and applies a random angular drift instead. The
 * drift is bounded by how far the current blind yaw (unit+1044) and pitch (unit+1048) have strayed from
 * their neutral cones (+/- pi/4 yaw, +/- pi/15 pitch), clamped to +/- pi/150 per tick, accumulated, wrapped
 * to [-pi,pi], and used to rotate the run vector about the world up axis. Returns nothing; writes run_vector.
 *
 * NOTE: the decompiler aliased the (unused) incoming long-double register a3 as scratch storage; it is a
 * pure local here (drift), never an input. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"
extern double cos(double x);
extern double sin(double x);


extern uint8_t actor_get_running_blind_vector(uint16_t actor_index, real_vector3d *run_vector);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);

void unit_running_blind(int unit_index, real_vector3d *run_vector)
{
    char have_actor_vector = 0;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int actor_index = unit->unit.actor_index;
    if ( actor_index == -1 || !actor_get_running_blind_vector(actor_index, run_vector) )
        *run_vector = *global_forward3d;
    else
        have_actor_vector = 1;

    double drift;
    float low_bound = 1.0f;
    float high_bound = 1.0f;
    float yaw = unit->unit.run_blindly_angle;
    float pitch = unit->unit.run_blindly_angle_delta;

    if ( have_actor_vector )
    {
        if ( ((0.78539819f - yaw) * 4.2441316f) < 1.0f )
            low_bound = ((0.78539819f - yaw) * 4.2441316f);
        if ( ((yaw + 0.78539819f) * 4.2441316f) < 1.0f )
            high_bound = ((yaw + 0.78539819f) * 4.2441316f);
    }

    if ( low_bound > ((0.20943952f - pitch) * 15.915494f) )
        low_bound = ((0.20943952f - pitch) * 15.915494f);
    if ( high_bound > ((pitch + 0.20943952f) * 15.915494f) )
        high_bound = ((pitch + 0.20943952f) * 15.915494f);

    if ( low_bound >= high_bound )
    {
        if ( high_bound < -1.0f )
        {
            drift = 0.020943951;
        }
        else
        {
            float frac = 1.0f;
            if ( high_bound < 1.0f )
                frac = high_bound;
            unsigned int *seed = get_global_random_seed_address();
            drift = real_seed_random_range(seed, (frac * -0.020943951f), 0.020943951f);
        }
    }
    else
    {
        if ( low_bound < -1.0f )
        {
            drift = -0.020943951;
        }
        else
        {
            float frac = 1.0f;
            if ( low_bound < 1.0f )
                frac = low_bound;
            unsigned int *seed = get_global_random_seed_address();
            drift = real_seed_random_range(seed, -0.020943951f, (frac * 0.020943951f));
        }
    }

    pitch = unit->unit.run_blindly_angle_delta;
    yaw = unit->unit.run_blindly_angle;
    float new_pitch = (pitch + (float)drift);
    unit->unit.run_blindly_angle_delta = new_pitch;
    float new_yaw = (yaw + new_pitch);
    unit->unit.run_blindly_angle = new_yaw;

    if ( new_yaw < -3.1415927f )
        unit->unit.run_blindly_angle = (new_yaw + TWO_PI);
    else if ( new_yaw > PI )
        unit->unit.run_blindly_angle = (new_yaw - TWO_PI);

    float final_yaw = unit->unit.run_blindly_angle;
    rotate_vector_about_axis(run_vector, global_up3d,
        (float)sin(final_yaw), (float)cos(final_yaw));
}
