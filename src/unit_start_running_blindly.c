/* unit_start_running_blindly @0x836CD3F0 — set a panicked unit's "run blindly" heading (unit.run_blindly_angle),
 * once (gated by unit flag bit 0x2000000). If the unit's actor supplies a preferred flee direction it
 * is used with a tight +-25 degree random spread; otherwise the unit flees along its current facing with a
 * wide +-100 degree spread. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t actor_get_running_blind_vector(uint16_t actor_index, real_vector3d *run_vector);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);

void unit_start_running_blindly(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unsigned int unit_flags = unit->unit.flags;
    if ( (unit_flags & (1u << _unit_running_blindly_bit)) != 0 )
        return;

    int actor_index = unit->unit.actor_index;
    unit->unit.flags = unit_flags | (1u << _unit_running_blindly_bit);

    float heading;
    float spread;
    real_vector3d blind_vector;
    if ( actor_index == -1 || !actor_get_running_blind_vector(actor_index, &blind_vector) )
    {
        real_euler_angles2d facing_angles;
        euler_angles2d_from_vector3d(&facing_angles, &unit->object.forward);
        heading = facing_angles.n[0];
        if ( facing_angles.n[0] > PI )
            heading = facing_angles.n[0] - TWO_PI;
        spread = 1.7453293f;   /* 100 degrees */
    }
    else
    {
        spread = 0.43633232f;  /* 25 degrees */
        heading = 0.0f;
    }

    unit->unit.run_blindly_angle = heading;
    unit->unit.run_blindly_angle += real_seed_random_range(get_global_random_seed_address(), -spread, spread);
}
