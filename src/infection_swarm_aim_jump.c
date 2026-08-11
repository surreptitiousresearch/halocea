/* infection_swarm_aim_jump @0x83828848 — compute the launch velocity for an infection-form ("swarm")
 * biped that wants to leap. Given the swarm actor and the specific member unit that is jumping, it finds
 * that member's swarm component, and either (a) solves a real ballistic arc toward the component's linked
 * jump prop via projectile_aim_ballistic, or (b) falls back to a flat directional hop derived from the
 * prop's / object's facing when no valid ballistic target exists. The resulting velocity is written to
 * *jump_velocity, clamped so its magnitude never exceeds the (>= 0.12) jump magnitude.
 *
 * Reconstructed from disassembly (0x83828848-0x83828B8C). The decompiler was reg-alloc-confused: it
 * conflated the component flags word (component+2) with the ballistic `origin` pointer and, crucially,
 * mis-mapped the projectile_aim_ballistic call. projectile_aim_ballistic takes its two float args in
 * f1/f2 which SHADOW r3/r4, so every pointer arg is shifted down two slots: origin=r5, target_point=r6,
 * target_velocity_min=r7, target_ballistic_fraction_min=r8, forced_velocity=r9, lob=r10, then six stack
 * out-params. Mapping the real register loads gives origin=component+4, target=prop+0xC8. jump_velocity
 * is the r6 output param (the decompiler's phantom FPR-shadow parameter a5). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/swarm_datum.h"
#include "headers/swarm_component_datum.h"
#include "headers/swarm_component_flags.h"
#include "headers/prop_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern float normalize2d(real_vector2d *v);
extern float __fsqrts(float x);
extern uint8_t projectile_aim_ballistic(float base_velocity, float gravity_scale, const real_point3d *origin, const real_point3d *target_point, float *target_velocity_min, float *target_ballistic_fraction_min, float *forced_velocity, uint8_t lob, real_vector3d *result_aim_vector, float *result_velocity, float *result_ticks, float *result_distance, float *result_vertical_velocity, float *result_horizontal_velocity);

void infection_swarm_aim_jump(int actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( actor->meta.swarm_cache_index == -1 )
        return;

    swarm_datum *swarm = DATUM_GET(swarm_data, swarm_datum, actor->meta.swarm_cache_index);
    if ( swarm->unit_count <= 0 )
        return;

    for ( int member_index = 0;
          member_index < swarm->unit_count;
          member_index = (int16_t)(member_index + 1) )
    {
        if ( swarm->unit_indices[member_index] != unit_index )
            continue;

        unit_datum *object_datum = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        swarm_component_datum *component = DATUM_GET(swarm_component_data, swarm_component_datum,
                swarm->component_indices[member_index]);
        uint16_t component_flags = component->flags;
        int prop_index = component->combat_target_prop_index;

        if ( (component_flags & (1u << _swarm_component_melee_engaged_bit)) != 0 && prop_index != -1 )
        {
            prop_datum *prop = DATUM_GET(prop_data, prop_datum, prop_index);
            float target_velocity_min = 0.059999999f;         /* r7 — loaded but ignored by callee */
            float target_ballistic_fraction_min = 0.80000001f; /* r8 */

            if ( jump_magnitude <= 0.12f )
                jump_magnitude = 0.12f;

            real_vector3d aim_direction;
            float vertical_velocity;
            float horizontal_velocity;
            if ( projectile_aim_ballistic(
                    jump_magnitude, 1.0f,
                    &component->position,
                    &prop->center_of_mass,
                    &target_velocity_min, &target_ballistic_fraction_min,
                    nullptr, 0, &aim_direction,
                    nullptr, nullptr, nullptr,
                    &vertical_velocity, &horizontal_velocity) )
            {
                if ( normalize2d((real_vector2d *)&aim_direction) == 0.0f )
                {
                    aim_direction.n[0] = actor->input.facing_vector.n[0];
                    aim_direction.n[1] = actor->input.facing_vector.n[1];
                    aim_direction.n[2] = actor->input.facing_vector.n[2];
                    if ( normalize2d((real_vector2d *)&aim_direction) == 0.0f )
                    {
                        aim_direction.n[0] = global_forward3d->n[0];
                        aim_direction.n[1] = global_forward3d->n[1];
                        aim_direction.n[2] = global_forward3d->n[2];
                    }
                }

                if ( !prop->flying && vertical_velocity > 0.075000003f )
                    vertical_velocity = 0.075000003f;

                float velocity_x = (aim_direction.n[0] * horizontal_velocity);
                float velocity_y = (aim_direction.n[1] * horizontal_velocity);
                jump_velocity->n[0] = velocity_x;
                jump_velocity->n[1] = velocity_y;
                jump_velocity->n[2] = vertical_velocity;

                float magnitude_squared = ((vertical_velocity * vertical_velocity)
                        + ((velocity_y * velocity_y) + (velocity_x * velocity_x)));
                if ( magnitude_squared > (jump_magnitude * jump_magnitude) )
                {
                    float magnitude = __fsqrts(magnitude_squared);
                    jump_velocity->n[0] = (velocity_x * (jump_magnitude / magnitude));
                    jump_velocity->n[1] = (velocity_y * (jump_magnitude / magnitude));
                    jump_velocity->n[2] = ((jump_magnitude / magnitude) * vertical_velocity);
                }
            }
        }
        else if ( (component_flags & (1u << _swarm_component_obey_direct_bit)) != 0
               && (component_flags & (1u << _swarm_component_jump_pending_bit)) != 0 )
        {
            if ( (component->___u9.obey.simple_control_flags & (1u << _obey_simple_control_jump_bit)) != 0
              && (component->___u9.obey.simple_control_flags & (1u << _obey_simple_control_jump_targeted_bit)) != 0 )
            {
                real_vector2d flat_direction;
                flat_direction = *(real_vector2d *)&object_datum->object.forward;
                if ( normalize2d(&flat_direction) == 0.0f )
                {
                    flat_direction = *(real_vector2d *)&object_datum->object.up;
                    if ( normalize2d(&flat_direction) == 0.0f )
                        flat_direction = *global_forward2d;
                }

                float speed = component->___u9.obey.___u5.jump.target_horizontal_vel;
                jump_velocity->n[0] = speed * flat_direction.n[0];
                jump_velocity->n[1] = speed * flat_direction.n[1];
                jump_velocity->n[2] = component->___u9.obey.___u5.jump.target_vertical_vel;
            }

            component->flags = component_flags & ~(1u << _swarm_component_jump_pending_bit);
        }
    }
}
