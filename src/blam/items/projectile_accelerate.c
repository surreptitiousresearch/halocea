/* projectile_accelerate @0x8375ADE8 — apply a damage impulse to a free (unparented) projectile: add the
 * acceleration to its translational_velocity and add a randomly-oriented angular kick proportional to
 * the impulse magnitude (up to pi/2 * random) to its angular_velocity, then re-solve its angular velocity
 * and clear the at-rest flag (flags bit 0x20). No-op if the projectile is attached. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern uint32_t *get_global_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern float real_seed_random(uint32_t *seed);
extern void projectile_adjust_for_angular_velocity_change(int object_index);

void projectile_accelerate(int projectile_index, const real_vector3d *acceleration)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    if ( object->object.parent_object_index != -1 )  /* attached projectile: ignore */
        return;

    object->object.translational_velocity.n[0] += acceleration->n[0];
    object->object.translational_velocity.n[1] += acceleration->n[1];
    object->object.translational_velocity.n[2] += acceleration->n[2];

    real_vector3d spin_axis;
    seed_random_direction3d(get_global_random_seed_address(), &spin_axis);
    float random = real_seed_random(get_global_random_seed_address());
    float magnitude = __fsqrts(acceleration->n[0] * acceleration->n[0]
                               + (acceleration->n[1] * acceleration->n[1]
                                  + acceleration->n[2] * acceleration->n[2]));
    float spin = magnitude * random * 1.5707964f;  /* up to pi/2 */
    spin_axis.n[0] *= spin;
    spin_axis.n[1] *= spin;
    spin_axis.n[2] *= spin;

    object->object.angular_velocity.n[0] += spin_axis.n[0];
    object->object.angular_velocity.n[1] += spin_axis.n[1];
    object->object.angular_velocity.n[2] += spin_axis.n[2];
    projectile_adjust_for_angular_velocity_change(projectile_index);
    object->object.flags &= ~(1u << _object_at_rest_bit);
}
