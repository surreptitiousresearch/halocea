/* biped_update_airborne @0x837AEA98 — per-frame airborne update for a biped. When the biped is flying
 * through the air and its definition tumbles in air (biped definition flag 0x100), and it is not
 * already in a dead/ragdoll-ish state (animation.state 31 or 41), a small random rotational impulse is added
 * to the biped's object.angular_velocity: the axis is the cross product of world up and the biped's
 * up vector (so it rights/tumbles toward upright) when that biped is tilted (up.z < 0.8), otherwise a
 * random horizontal direction; the magnitude is ~3-5 degrees. biped_apply_rotation integrates it.
 * Finally the desired animation state is selected: 40 while landing (state 39/40), else 20 while
 * jumping (state 20) or otherwise airborne. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_update_data.h"
#include "headers/real_vector3d.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"


extern uint8_t biped_flying_through_air(int biped_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern void biped_apply_rotation(int biped_index);

void biped_update_airborne(int biped_index, unit_animation_update_data *animation)
{
    char airborne = 0;
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    if (biped_flying_through_air(biped_index))
    {
        airborne = 1;
        biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
        if ((definition->biped.flags & (1u << _biped_rotate_while_airborne_bit)) != 0)
        {
            int state = biped->unit.animation.state;
            if (state != _unit_state_melee_airborne && state != _unit_state_leap_melee)
            {
                float impulse = real_seed_random_range(get_global_random_seed_address(),
                                                       0.052359879f, 0.08726646f);
                const float *biped_up = biped->object.up.n;
                real_vector3d axis;
                int have_axis = 0;
                if (biped_up[2] < 0.80000001f)
                {
                    /* axis = world_up x biped_up : rights the biped toward upright */
                    axis.n[0] = global_up3d->n[2] * biped_up[1] - global_up3d->n[1] * biped_up[2];
                    axis.n[1] = global_up3d->n[0] * biped_up[2] - global_up3d->n[2] * biped_up[0];
                    axis.n[2] = global_up3d->n[1] * biped_up[0] - global_up3d->n[0] * biped_up[1];
                    have_axis = normalize3d(&axis) > 0.0f;
                }
                if (!have_axis)
                {
                    float angle = real_seed_random_range(get_global_random_seed_address(), 0.0f, TWO_PI);
                    vector3d_from_angle(&axis, angle);
                }
                biped->object.angular_velocity.n[0] += axis.n[0] * impulse;
                biped->object.angular_velocity.n[1] += axis.n[1] * impulse;
                biped->object.angular_velocity.n[2] += axis.n[2] * impulse;
            }
            biped_apply_rotation(biped_index);
        }
    }

    int state = biped->unit.animation.state;
    if (state == _unit_state_leap_start || state == _unit_state_leap_airborne)
        animation->state_desired = _unit_state_leap_airborne;
    else if (state == _unit_state_airborne || airborne)
        animation->state_desired = _unit_state_airborne;
}
